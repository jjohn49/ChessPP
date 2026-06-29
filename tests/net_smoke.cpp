//
// net_smoke — manual loopback smoke test for NetSession (NOT a unit test; it
// uses real sockets/threads). Hosts on 127.0.0.1, joins itself, exchanges
// messages both ways, verifies back-to-back framing, and a clean shutdown.
//
// Build:  make NetSmoke   (from the build dir)
// Run:    ./NetSmoke      (exit code 0 = pass)
//

#include "../src/Headers/NetSession.h"
#include "../src/Headers/NetProtocol.h"

#include <iostream>
#include <thread>
#include <chrono>

static std::string spinPoll(NetSession & s, int tries = 200) {
	for (int i = 0; i < tries; i++) {
		if (auto m = s.poll()) return *m;
		std::this_thread::sleep_for(std::chrono::milliseconds(5));
	}
	return "";
}

int main() {
	const std::uint16_t port = 53999;

	NetSession host;
	if (!host.host(port)) {
		std::cerr << "host() failed\n";
		return 1;
	}

	bool accepted = false;
	std::thread acceptor([&] { accepted = host.acceptOne(3000); });

	NetSession joiner;
	bool joined = joiner.join("127.0.0.1", port, 3000);
	acceptor.join();

	if (!joined || !accepted) {
		std::cerr << "connect failed (joined=" << joined << " accepted=" << accepted << ")\n";
		return 1;
	}
	std::cout << "connected OK\n";

	bool ok = true;

	// Bidirectional single messages.
	joiner.send(NetProtocol::serializeMove({1, 4, 3, 4, '-'}));
	host.send(NetProtocol::serializeHello(NetProtocol::kProtocolVersion));

	std::string hostGot = spinPoll(host);
	std::string joinerGot = spinPoll(joiner);
	std::cout << "host received:   [" << hostGot << "]\n";
	std::cout << "joiner received: [" << joinerGot << "]\n";
	ok = ok && hostGot == "MOVE 1 4 3 4 -";
	ok = ok && joinerGot == NetProtocol::serializeHello(1);

	// Back-to-back framing: two messages sent together must arrive as two lines.
	joiner.send("PING 1");
	joiner.send("PING 2");
	std::string a = spinPoll(host);
	std::string b = spinPoll(host);
	std::cout << "framing: [" << a << "] [" << b << "]\n";
	ok = ok && a == "PING 1" && b == "PING 2";

	host.close();
	joiner.close();

	std::cout << (ok ? "SMOKE PASS\n" : "SMOKE FAIL\n");
	return ok ? 0 : 1;
}
