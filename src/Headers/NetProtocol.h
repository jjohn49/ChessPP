//
// NetProtocol — the LAN multiplayer wire format. Pure (de)serialization of
// newline-delimited, space-separated text messages. No sockets, no SDL, no
// dependency on Board/Piece, so it can be exhaustively unit-tested.
//
// Piece-related values are encoded as explicit characters ('Q','R','N' for
// promotion, 'W'/'B' for color) rather than raw enum ordinals, so the format is
// stable regardless of the Piece::Type / Piece::Color enum ordering.
//

#ifndef CHESSPP_NETPROTOCOL_H
#define CHESSPP_NETPROTOCOL_H

#include <string>
#include <optional>
#include <vector>

namespace NetProtocol {
	constexpr int kProtocolVersion = 1;

	enum class Type {
		Unknown, Hello, Assign, Move, Pickup, Drag, Drop, State, Ping, Pong
	};

	// Classify a received line by its leading token.
	Type messageType(const std::string & line);

	// A move on the wire: board coordinates plus a promotion choice.
	// promo is one of '-' (none), 'Q', 'R', 'N' (matching the promotion popup,
	// which never offers bishop).
	struct NetMove {
		int fromRow{};
		int fromCol{};
		int toRow{};
		int toCol{};
		char promo{'-'};
		bool operator==(const NetMove & o) const {
			return fromRow == o.fromRow && fromCol == o.fromCol
			    && toRow == o.toRow && toCol == o.toCol && promo == o.promo;
		}
	};

	struct Assign {
		char colorChar{};   // 'W' or 'B' — the joiner's assigned color
		int tcSeconds{};    // time control in seconds, -1 = no clock
	};

	struct Square { int row{}; int col{}; };
	struct DragPos { double rowF{}; double colF{}; };

	struct State {
		char turn{'W'};            // whose move it is, 'W' or 'B'
		double whiteTime{};        // seconds remaining (advisory)
		double blackTime{};
		std::vector<NetMove> moves; // full authoritative move history for replay
	};

	std::string serializeHello(int version);
	std::optional<int> parseHello(const std::string & line);

	std::string serializeAssign(char colorChar, int tcSeconds);
	std::optional<Assign> parseAssign(const std::string & line);

	std::string serializeMove(const NetMove & m);
	std::optional<NetMove> parseMove(const std::string & line);

	std::string serializePickup(int row, int col);
	std::optional<Square> parsePickup(const std::string & line);

	std::string serializeDrag(double rowF, double colF);
	std::optional<DragPos> parseDrag(const std::string & line);

	std::string serializeDrop();

	std::string serializeState(const State & s);
	std::optional<State> parseState(const std::string & line);

	std::string serializePing(long long epochMs);
	std::optional<long long> parsePing(const std::string & line);
	std::string serializePong(long long epochMs);
	std::optional<long long> parsePong(const std::string & line);
}

#endif //CHESSPP_NETPROTOCOL_H
