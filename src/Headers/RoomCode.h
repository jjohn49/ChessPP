//
// RoomCode — pure helpers for turning a host's LAN IPv4 address into a short,
// human-typeable room code and back. No sockets, no SDL: trivially unit-testable.
//

#ifndef CHESSPP_ROOMCODE_H
#define CHESSPP_ROOMCODE_H

#include <cstdint>
#include <string>
#include <optional>

namespace RoomCode {
	// Fixed LAN port the host listens on. Baking it in keeps the room code short
	// (it only has to carry the IPv4 address) and keeps the macOS firewall
	// allow-rule stable across runs.
	constexpr std::uint16_t kLanPort = 52525;

	// Encode a 32-bit IPv4 address into a fixed 7-character Crockford base32 code.
	std::string encode(std::uint32_t ipv4);

	// Decode a room code back into an IPv4 address. Case-insensitive and tolerant
	// of Crockford's confusable characters (I/L -> 1, O -> 0). Returns nullopt on
	// malformed input (bad length, illegal character, or 32-bit overflow).
	std::optional<std::uint32_t> decode(const std::string & code);

	// Parse / format dotted-decimal IPv4 ("192.168.1.42"). Returns nullopt for
	// anything that is not four 0-255 octets.
	std::optional<std::uint32_t> ipv4FromString(const std::string & s);
	std::string ipv4ToString(std::uint32_t ipv4);
}

#endif //CHESSPP_ROOMCODE_H
