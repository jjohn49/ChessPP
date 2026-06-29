//
// NetProtocol — (de)serialization of the LAN multiplayer wire format. See
// NetProtocol.h. Messages are space-separated tokens; parsers validate the
// leading type token, arity, and field types, returning nullopt on any mismatch.
//

#include "../Headers/NetProtocol.h"
#include <sstream>
#include <iomanip>

namespace {
	std::vector<std::string> tokenize(const std::string & line) {
		std::vector<std::string> out;
		std::istringstream ss(line);
		std::string tok;
		while (ss >> tok) out.push_back(tok);
		return out;
	}

	std::optional<int> toInt(const std::string & s) {
		try {
			size_t idx;
			int v = std::stoi(s, &idx);
			if (idx != s.size()) return std::nullopt;
			return v;
		} catch (...) { return std::nullopt; }
	}

	std::optional<long long> toLL(const std::string & s) {
		try {
			size_t idx;
			long long v = std::stoll(s, &idx);
			if (idx != s.size()) return std::nullopt;
			return v;
		} catch (...) { return std::nullopt; }
	}

	std::optional<double> toDouble(const std::string & s) {
		try {
			size_t idx;
			double v = std::stod(s, &idx);
			if (idx != s.size()) return std::nullopt;
			return v;
		} catch (...) { return std::nullopt; }
	}

	bool isValidPromo(const std::string & s) {
		return s.size() == 1 && (s[0] == '-' || s[0] == 'Q' || s[0] == 'R' || s[0] == 'N');
	}

	std::string fmtDouble(double v) {
		std::ostringstream ss;
		ss << std::fixed << std::setprecision(3) << v;
		return ss.str();
	}
}

namespace NetProtocol {

Type messageType(const std::string & line) {
	auto t = tokenize(line);
	if (t.empty()) return Type::Unknown;
	const std::string & h = t[0];
	if (h == "HELLO")  return Type::Hello;
	if (h == "ASSIGN") return Type::Assign;
	if (h == "MOVE")   return Type::Move;
	if (h == "PICKUP") return Type::Pickup;
	if (h == "DRAG")   return Type::Drag;
	if (h == "DROP")   return Type::Drop;
	if (h == "STATE")  return Type::State;
	if (h == "PING")   return Type::Ping;
	if (h == "PONG")   return Type::Pong;
	return Type::Unknown;
}

std::string serializeHello(int version) {
	return "HELLO " + std::to_string(version);
}
std::optional<int> parseHello(const std::string & line) {
	auto t = tokenize(line);
	if (t.size() != 2 || t[0] != "HELLO") return std::nullopt;
	return toInt(t[1]);
}

std::string serializeAssign(char colorChar, int tcSeconds) {
	std::string s = "ASSIGN ";
	s += colorChar;
	s += " " + std::to_string(tcSeconds);
	return s;
}
std::optional<Assign> parseAssign(const std::string & line) {
	auto t = tokenize(line);
	if (t.size() != 3 || t[0] != "ASSIGN") return std::nullopt;
	if (t[1].size() != 1 || (t[1][0] != 'W' && t[1][0] != 'B')) return std::nullopt;
	auto tc = toInt(t[2]);
	if (!tc) return std::nullopt;
	return Assign{t[1][0], *tc};
}

std::string serializeMove(const NetMove & m) {
	std::ostringstream ss;
	ss << "MOVE " << m.fromRow << ' ' << m.fromCol << ' '
	   << m.toRow << ' ' << m.toCol << ' ' << m.promo;
	return ss.str();
}
std::optional<NetMove> parseMove(const std::string & line) {
	auto t = tokenize(line);
	if (t.size() != 6 || t[0] != "MOVE") return std::nullopt;
	auto fr = toInt(t[1]); auto fc = toInt(t[2]);
	auto tr = toInt(t[3]); auto tc = toInt(t[4]);
	if (!fr || !fc || !tr || !tc) return std::nullopt;
	if (!isValidPromo(t[5])) return std::nullopt;
	return NetMove{*fr, *fc, *tr, *tc, t[5][0]};
}

std::string serializePickup(int row, int col) {
	return "PICKUP " + std::to_string(row) + " " + std::to_string(col);
}
std::optional<Square> parsePickup(const std::string & line) {
	auto t = tokenize(line);
	if (t.size() != 3 || t[0] != "PICKUP") return std::nullopt;
	auto r = toInt(t[1]); auto c = toInt(t[2]);
	if (!r || !c) return std::nullopt;
	return Square{*r, *c};
}

std::string serializeDrag(double rowF, double colF) {
	return "DRAG " + fmtDouble(rowF) + " " + fmtDouble(colF);
}
std::optional<DragPos> parseDrag(const std::string & line) {
	auto t = tokenize(line);
	if (t.size() != 3 || t[0] != "DRAG") return std::nullopt;
	auto r = toDouble(t[1]); auto c = toDouble(t[2]);
	if (!r || !c) return std::nullopt;
	return DragPos{*r, *c};
}

std::string serializeDrop() { return "DROP"; }

std::string serializeState(const State & s) {
	std::ostringstream ss;
	ss << "STATE " << s.turn << ' ' << fmtDouble(s.whiteTime) << ' '
	   << fmtDouble(s.blackTime) << ' ' << s.moves.size();
	for (const auto & m : s.moves) {
		ss << ' ' << m.fromRow << ' ' << m.fromCol << ' '
		   << m.toRow << ' ' << m.toCol << ' ' << m.promo;
	}
	return ss.str();
}
std::optional<State> parseState(const std::string & line) {
	auto t = tokenize(line);
	if (t.size() < 5 || t[0] != "STATE") return std::nullopt;
	if (t[1].size() != 1 || (t[1][0] != 'W' && t[1][0] != 'B')) return std::nullopt;
	auto wt = toDouble(t[2]); auto bt = toDouble(t[3]); auto cnt = toInt(t[4]);
	if (!wt || !bt || !cnt || *cnt < 0) return std::nullopt;
	if (t.size() != static_cast<size_t>(5 + (*cnt) * 5)) return std::nullopt;
	State s;
	s.turn = t[1][0];
	s.whiteTime = *wt;
	s.blackTime = *bt;
	for (int j = 0; j < *cnt; j++) {
		int base = 5 + j * 5;
		auto fr = toInt(t[base]);   auto fc = toInt(t[base + 1]);
		auto tr = toInt(t[base + 2]); auto tc = toInt(t[base + 3]);
		if (!fr || !fc || !tr || !tc) return std::nullopt;
		if (!isValidPromo(t[base + 4])) return std::nullopt;
		s.moves.push_back(NetMove{*fr, *fc, *tr, *tc, t[base + 4][0]});
	}
	return s;
}

std::string serializePing(long long epochMs) { return "PING " + std::to_string(epochMs); }
std::optional<long long> parsePing(const std::string & line) {
	auto t = tokenize(line);
	if (t.size() != 2 || t[0] != "PING") return std::nullopt;
	return toLL(t[1]);
}
std::string serializePong(long long epochMs) { return "PONG " + std::to_string(epochMs); }
std::optional<long long> parsePong(const std::string & line) {
	auto t = tokenize(line);
	if (t.size() != 2 || t[0] != "PONG") return std::nullopt;
	return toLL(t[1]);
}

}
