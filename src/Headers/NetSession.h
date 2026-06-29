//
// NetSession — a single peer-to-peer TCP connection for LAN multiplayer.
// One side hosts (binds + listens + accepts one client), the other joins.
// A background reader thread frames incoming newline-delimited messages into a
// mutex-protected inbox the main loop drains each frame; sends happen on the
// main thread. Low latency via TCP_NODELAY. POSIX sockets only — no SDL.
//

#ifndef CHESSPP_NETSESSION_H
#define CHESSPP_NETSESSION_H

#include <string>
#include <optional>
#include <thread>
#include <mutex>
#include <atomic>
#include <deque>
#include <cstdint>

class NetSession {
public:
	enum class Role { None, Host, Joiner };

	NetSession() = default;
	~NetSession();

	NetSession(const NetSession &) = delete;
	NetSession & operator=(const NetSession &) = delete;

	// Host: create a listening socket on `port` (SO_REUSEADDR). False on failure.
	bool host(std::uint16_t port);
	// Host: wait up to timeoutMs for a client. On success starts the reader and
	// marks the session connected. False on timeout/error.
	bool acceptOne(int timeoutMs);

	// Joiner: connect to ip:port within timeoutMs. Starts the reader on success.
	bool join(const std::string & ip, std::uint16_t port, int timeoutMs);

	// Send a message body; a single '\n' frame terminator is appended. Returns
	// false if not connected or the write failed (which marks the session down).
	bool send(const std::string & body);

	// Non-blocking: pop the next complete inbound line (newline stripped) or nullopt.
	std::optional<std::string> poll();

	bool isConnected() const { return connected_.load(); }
	Role role() const { return role_; }

	// Re-establish a dropped connection: host re-accepts on its existing listener;
	// joiner re-dials the last ip:port. Returns true once reconnected.
	bool reconnect(int totalTimeoutMs);

	void close();

	// Best-effort LAN IPv4 discovery (first non-loopback private AF_INET). 0 if none.
	static std::uint32_t getLocalLanIpRaw();
	static std::string getLocalLanIp();

private:
	Role role_ = Role::None;
	int listenFd_ = -1;
	int connFd_ = -1;
	std::string remoteIp_;
	std::uint16_t remotePort_ = 0;

	std::thread reader_;
	std::atomic<bool> connected_{false};
	std::atomic<bool> stop_{false};

	std::mutex inboxMx_;
	std::deque<std::string> inbox_;

	void startReader();
	void stopReader();
	void readerLoop();
	static void configureSocket(int fd);   // TCP_NODELAY
};

#endif //CHESSPP_NETSESSION_H
