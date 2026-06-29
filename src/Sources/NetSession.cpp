//
// NetSession — POSIX TCP implementation. See NetSession.h.
//

#include "../Headers/NetSession.h"
#include "../Headers/RoomCode.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/select.h>
#include <cerrno>
#include <cstring>

NetSession::~NetSession() {
	close();
}

void NetSession::configureSocket(int fd) {
	int one = 1;
	setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, &one, sizeof(one));
}

bool NetSession::host(std::uint16_t port) {
	listenFd_ = ::socket(AF_INET, SOCK_STREAM, 0);
	if (listenFd_ < 0) return false;

	int one = 1;
	setsockopt(listenFd_, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one));

	sockaddr_in addr{};
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(port);

	if (::bind(listenFd_, reinterpret_cast<sockaddr *>(&addr), sizeof(addr)) < 0) {
		::close(listenFd_);
		listenFd_ = -1;
		return false;
	}
	if (::listen(listenFd_, 1) < 0) {
		::close(listenFd_);
		listenFd_ = -1;
		return false;
	}
	role_ = Role::Host;
	return true;
}

bool NetSession::acceptOne(int timeoutMs) {
	if (listenFd_ < 0) return false;

	fd_set rset;
	FD_ZERO(&rset);
	FD_SET(listenFd_, &rset);
	timeval tv{timeoutMs / 1000, (timeoutMs % 1000) * 1000};

	int s = ::select(listenFd_ + 1, &rset, nullptr, nullptr, &tv);
	if (s <= 0) return false;   // timeout or error

	int fd = ::accept(listenFd_, nullptr, nullptr);
	if (fd < 0) return false;

	connFd_ = fd;
	configureSocket(connFd_);
	connected_.store(true);
	startReader();
	return true;
}

bool NetSession::join(const std::string & ip, std::uint16_t port, int timeoutMs) {
	int fd = ::socket(AF_INET, SOCK_STREAM, 0);
	if (fd < 0) return false;

	sockaddr_in addr{};
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	if (::inet_pton(AF_INET, ip.c_str(), &addr.sin_addr) != 1) {
		::close(fd);
		return false;
	}

	// Non-blocking connect with a select() timeout, then restore blocking mode.
	int flags = ::fcntl(fd, F_GETFL, 0);
	::fcntl(fd, F_SETFL, flags | O_NONBLOCK);

	int rc = ::connect(fd, reinterpret_cast<sockaddr *>(&addr), sizeof(addr));
	if (rc < 0 && errno != EINPROGRESS) {
		::close(fd);
		return false;
	}
	if (rc < 0) {
		fd_set wset;
		FD_ZERO(&wset);
		FD_SET(fd, &wset);
		timeval tv{timeoutMs / 1000, (timeoutMs % 1000) * 1000};
		if (::select(fd + 1, nullptr, &wset, nullptr, &tv) <= 0) {
			::close(fd);
			return false;
		}
		int err = 0;
		socklen_t len = sizeof(err);
		if (::getsockopt(fd, SOL_SOCKET, SO_ERROR, &err, &len) < 0 || err != 0) {
			::close(fd);
			return false;
		}
	}
	::fcntl(fd, F_SETFL, flags);   // back to blocking

	connFd_ = fd;
	configureSocket(connFd_);
	remoteIp_ = ip;
	remotePort_ = port;
	role_ = Role::Joiner;
	connected_.store(true);
	startReader();
	return true;
}

bool NetSession::send(const std::string & body) {
	if (!connected_.load() || connFd_ < 0) return false;

	std::string msg = body;
	msg.push_back('\n');

	const char * p = msg.data();
	size_t remaining = msg.size();
	while (remaining > 0) {
		ssize_t n = ::send(connFd_, p, remaining, 0);
		if (n <= 0) {
			if (n < 0 && errno == EINTR) continue;
			connected_.store(false);
			return false;
		}
		p += n;
		remaining -= static_cast<size_t>(n);
	}
	return true;
}

std::optional<std::string> NetSession::poll() {
	std::lock_guard<std::mutex> lock(inboxMx_);
	if (inbox_.empty()) return std::nullopt;
	std::string line = std::move(inbox_.front());
	inbox_.pop_front();
	return line;
}

void NetSession::startReader() {
	stop_.store(false);
	reader_ = std::thread(&NetSession::readerLoop, this);
}

void NetSession::stopReader() {
	stop_.store(true);
	if (connFd_ >= 0) {
		::shutdown(connFd_, SHUT_RDWR);   // unblock a blocking recv()
	}
	if (reader_.joinable()) {
		reader_.join();
	}
	if (connFd_ >= 0) {
		::close(connFd_);
		connFd_ = -1;
	}
}

void NetSession::readerLoop() {
	char buf[4096];
	std::string accum;

	while (!stop_.load()) {
		ssize_t n = ::recv(connFd_, buf, sizeof(buf), 0);
		if (n > 0) {
			accum.append(buf, static_cast<size_t>(n));
			size_t nl;
			while ((nl = accum.find('\n')) != std::string::npos) {
				std::string line = accum.substr(0, nl);
				accum.erase(0, nl + 1);
				if (!line.empty() && line.back() == '\r') line.pop_back();
				{
					std::lock_guard<std::mutex> lock(inboxMx_);
					inbox_.push_back(std::move(line));
				}
			}
		} else if (n == 0) {
			connected_.store(false);   // peer closed
			break;
		} else {
			if (errno == EINTR) continue;
			connected_.store(false);
			break;
		}
	}
}

bool NetSession::reconnect(int totalTimeoutMs) {
	stopReader();
	stop_.store(false);

	if (role_ == Role::Host) {
		return acceptOne(totalTimeoutMs);
	}
	if (role_ == Role::Joiner) {
		int remaining = totalTimeoutMs;
		while (remaining > 0) {
			int slice = remaining > 500 ? 500 : remaining;
			if (join(remoteIp_, remotePort_, slice)) return true;
			remaining -= slice;
		}
		return false;
	}
	return false;
}

void NetSession::close() {
	stopReader();
	if (listenFd_ >= 0) {
		::close(listenFd_);
		listenFd_ = -1;
	}
	connected_.store(false);
}

std::uint32_t NetSession::getLocalLanIpRaw() {
	std::uint32_t fallback = 0;
	std::uint32_t priv = 0;

	ifaddrs * ifaList = nullptr;
	if (::getifaddrs(&ifaList) == 0) {
		for (ifaddrs * ifa = ifaList; ifa; ifa = ifa->ifa_next) {
			if (!ifa->ifa_addr || ifa->ifa_addr->sa_family != AF_INET) continue;
			auto * sin = reinterpret_cast<sockaddr_in *>(ifa->ifa_addr);
			std::uint32_t ip = ntohl(sin->sin_addr.s_addr);
			std::uint8_t a = (ip >> 24) & 0xFF;
			std::uint8_t b = (ip >> 16) & 0xFF;
			if (a == 127) continue;   // skip loopback
			if (fallback == 0) fallback = ip;
			bool isPrivate = (a == 10) || (a == 192 && b == 168) || (a == 172 && b >= 16 && b <= 31);
			if (isPrivate && priv == 0) priv = ip;
		}
		::freeifaddrs(ifaList);
	}
	return priv ? priv : fallback;
}

std::string NetSession::getLocalLanIp() {
	return RoomCode::ipv4ToString(getLocalLanIpRaw());
}
