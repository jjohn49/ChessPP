//
// RoomCode — encode a host's LAN IPv4 into a short Crockford base32 room code
// and back. See RoomCode.h.
//

#include "../Headers/RoomCode.h"
#include <array>
#include <cctype>

namespace {
	// Crockford base32 alphabet (excludes I, L, O, U to avoid confusion).
	const std::string kAlphabet = "0123456789ABCDEFGHJKMNPQRSTVWXYZ";

	// Map a code character to its 5-bit value, applying Crockford's confusable
	// normalization (I/L -> 1, O -> 0) and case-folding. Returns -1 if illegal.
	int crockfordValue(char c) {
		c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
		if (c == 'I' || c == 'L') c = '1';
		if (c == 'O') c = '0';
		auto pos = kAlphabet.find(c);
		return (pos == std::string::npos) ? -1 : static_cast<int>(pos);
	}

	std::optional<int> parseOctet(const std::string & s) {
		if (s.empty() || s.size() > 3) return std::nullopt;
		int v = 0;
		for (char c : s) {
			if (!std::isdigit(static_cast<unsigned char>(c))) return std::nullopt;
			v = v * 10 + (c - '0');
		}
		if (v > 255) return std::nullopt;
		return v;
	}
}

namespace RoomCode {

std::string encode(std::uint32_t ipv4) {
	// 7 base32 digits carry 35 bits; the IPv4 fits in the low 32 (top 3 bits zero).
	std::string out(7, '0');
	for (int i = 0; i < 7; i++) {
		int shift = 5 * (6 - i);
		int digit = static_cast<int>((static_cast<std::uint64_t>(ipv4) >> shift) & 0x1F);
		out[i] = kAlphabet[digit];
	}
	return out;
}

std::optional<std::uint32_t> decode(const std::string & code) {
	if (code.size() != 7) return std::nullopt;
	std::uint64_t value = 0;
	for (char c : code) {
		int v = crockfordValue(c);
		if (v < 0) return std::nullopt;
		value = value * 32 + static_cast<std::uint64_t>(v);
	}
	if (value > 0xFFFFFFFFull) return std::nullopt;   // would not fit in 32 bits
	return static_cast<std::uint32_t>(value);
}

std::optional<std::uint32_t> ipv4FromString(const std::string & s) {
	std::array<int, 4> octets{};
	int part = 0;
	std::string cur;
	for (char c : s) {
		if (c == '.') {
			if (part >= 3) return std::nullopt;   // more than three dots
			auto o = parseOctet(cur);
			if (!o) return std::nullopt;
			octets[part++] = *o;
			cur.clear();
		} else {
			cur.push_back(c);
		}
	}
	if (part != 3) return std::nullopt;            // need exactly three dots
	auto o = parseOctet(cur);
	if (!o) return std::nullopt;
	octets[3] = *o;
	return (static_cast<std::uint32_t>(octets[0]) << 24)
	     | (static_cast<std::uint32_t>(octets[1]) << 16)
	     | (static_cast<std::uint32_t>(octets[2]) << 8)
	     | static_cast<std::uint32_t>(octets[3]);
}

std::string ipv4ToString(std::uint32_t ipv4) {
	return std::to_string((ipv4 >> 24) & 0xFF) + "." +
	       std::to_string((ipv4 >> 16) & 0xFF) + "." +
	       std::to_string((ipv4 >> 8) & 0xFF) + "." +
	       std::to_string(ipv4 & 0xFF);
}

}
