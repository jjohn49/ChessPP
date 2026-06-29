//
// Unit tests for the pure networking layer: RoomCode + NetProtocol.
// No sockets, no SDL — pure (de)serialization round-trips and rejection cases.
//

#include "TestFramework.h"
#include "../src/Headers/RoomCode.h"
#include "../src/Headers/NetProtocol.h"
#include <cmath>
#include <cctype>

using namespace NetProtocol;

// --- RoomCode: dotted-decimal parsing ---

TEST(RoomCode_IpStringRoundTrip) {
	auto ip = RoomCode::ipv4FromString("192.168.1.42");
	ASSERT_TRUE(ip.has_value());
	ASSERT_EQ(RoomCode::ipv4ToString(ip.value()), std::string("192.168.1.42"));
}

TEST(RoomCode_IpStringRejectsMalformed) {
	ASSERT_FALSE(RoomCode::ipv4FromString("192.168.1").has_value());
	ASSERT_FALSE(RoomCode::ipv4FromString("192.168.1.256").has_value());
	ASSERT_FALSE(RoomCode::ipv4FromString("a.b.c.d").has_value());
	ASSERT_FALSE(RoomCode::ipv4FromString("1.2.3.4.5").has_value());
	ASSERT_FALSE(RoomCode::ipv4FromString("").has_value());
}

// --- RoomCode: code encode/decode ---

TEST(RoomCode_EncodeDecodeRoundTrip) {
	const char * ips[] = {"192.168.1.42", "10.0.0.1", "172.16.5.200",
	                      "255.255.255.255", "0.0.0.0", "127.0.0.1"};
	for (auto s : ips) {
		std::uint32_t ip = RoomCode::ipv4FromString(s).value();
		std::string code = RoomCode::encode(ip);
		auto decoded = RoomCode::decode(code);
		ASSERT_TRUE(decoded.has_value());
		ASSERT_EQ(decoded.value(), ip);
	}
}

TEST(RoomCode_EncodeLengthAndCharset) {
	std::string code = RoomCode::encode(RoomCode::ipv4FromString("192.168.1.42").value());
	ASSERT_EQ(code.size(), static_cast<size_t>(7));
	std::string alphabet = "0123456789ABCDEFGHJKMNPQRSTVWXYZ";
	for (char c : code) {
		ASSERT_TRUE(alphabet.find(c) != std::string::npos);
	}
}

TEST(RoomCode_DecodeCaseInsensitive) {
	std::uint32_t ip = RoomCode::ipv4FromString("192.168.1.42").value();
	std::string lower = RoomCode::encode(ip);
	for (char & c : lower) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
	auto decoded = RoomCode::decode(lower);
	ASSERT_TRUE(decoded.has_value());
	ASSERT_EQ(decoded.value(), ip);
}

TEST(RoomCode_DecodeRejectsGarbage) {
	ASSERT_FALSE(RoomCode::decode("").has_value());
	ASSERT_FALSE(RoomCode::decode("!!!!!!!").has_value());      // illegal chars
	ASSERT_FALSE(RoomCode::decode("ABC").has_value());          // too short
	ASSERT_FALSE(RoomCode::decode("ABCDEFGHJKM").has_value());  // too long
}

// --- NetProtocol: message classification ---

TEST(NetProtocol_MessageType) {
	ASSERT_TRUE(messageType("HELLO 1") == Type::Hello);
	ASSERT_TRUE(messageType("ASSIGN W 300") == Type::Assign);
	ASSERT_TRUE(messageType("MOVE 1 4 3 4 -") == Type::Move);
	ASSERT_TRUE(messageType("PICKUP 1 4") == Type::Pickup);
	ASSERT_TRUE(messageType("DRAG 1.5 4.0") == Type::Drag);
	ASSERT_TRUE(messageType("DROP") == Type::Drop);
	ASSERT_TRUE(messageType("STATE W 300 300 0") == Type::State);
	ASSERT_TRUE(messageType("PING 123") == Type::Ping);
	ASSERT_TRUE(messageType("PONG 123") == Type::Pong);
	ASSERT_TRUE(messageType("GARBAGE x") == Type::Unknown);
	ASSERT_TRUE(messageType("") == Type::Unknown);
}

// --- NetProtocol: round-trips ---

TEST(NetProtocol_HelloRoundTrip) {
	auto v = parseHello(serializeHello(kProtocolVersion));
	ASSERT_TRUE(v.has_value());
	ASSERT_EQ(v.value(), kProtocolVersion);
}

TEST(NetProtocol_AssignRoundTrip) {
	auto a = parseAssign(serializeAssign('B', 300));
	ASSERT_TRUE(a.has_value());
	ASSERT_EQ(a.value().colorChar, 'B');
	ASSERT_EQ(a.value().tcSeconds, 300);

	auto a2 = parseAssign(serializeAssign('W', -1));
	ASSERT_TRUE(a2.has_value());
	ASSERT_EQ(a2.value().colorChar, 'W');
	ASSERT_EQ(a2.value().tcSeconds, -1);
}

TEST(NetProtocol_MoveRoundTrip) {
	NetMove moves[] = {
		{1, 4, 3, 4, '-'},   // pawn double push e2-e4
		{6, 4, 4, 4, '-'},   // a capture serializes identically (from/to only)
		{0, 4, 0, 6, '-'},   // kingside castle expressed as a king move e1-g1
		{0, 4, 0, 2, '-'},   // queenside castle e1-c1
		{4, 4, 5, 3, '-'},   // en passant target square
		{6, 0, 7, 0, 'Q'},   // promote to queen
		{6, 1, 7, 1, 'R'},   // promote to rook
		{6, 2, 7, 2, 'N'},   // promote to knight
	};
	for (auto & m : moves) {
		auto parsed = parseMove(serializeMove(m));
		ASSERT_TRUE(parsed.has_value());
		ASSERT_TRUE(parsed.value() == m);
	}
}

TEST(NetProtocol_MoveRejectsMalformed) {
	ASSERT_FALSE(parseMove("MOVE 1 4 3").has_value());      // too few tokens
	ASSERT_FALSE(parseMove("MOVE a b c d -").has_value());  // non-numeric coords
	ASSERT_FALSE(parseMove("MOVE 1 4 3 4 X").has_value());  // illegal promo char
	ASSERT_FALSE(parseMove("PICKUP 1 4").has_value());      // wrong message type
}

TEST(NetProtocol_PickupRoundTrip) {
	auto p = parsePickup(serializePickup(2, 5));
	ASSERT_TRUE(p.has_value());
	ASSERT_EQ(p.value().row, 2);
	ASSERT_EQ(p.value().col, 5);
}

TEST(NetProtocol_DragRoundTrip) {
	auto d = parseDrag(serializeDrag(3.5, 4.25));
	ASSERT_TRUE(d.has_value());
	ASSERT_TRUE(std::fabs(d.value().rowF - 3.5) < 0.01);
	ASSERT_TRUE(std::fabs(d.value().colF - 4.25) < 0.01);
}

TEST(NetProtocol_DropType) {
	ASSERT_TRUE(messageType(serializeDrop()) == Type::Drop);
}

TEST(NetProtocol_StateRoundTrip) {
	State s;
	s.turn = 'B';
	s.whiteTime = 295.5;
	s.blackTime = 300.0;
	s.moves = {{1, 4, 3, 4, '-'}, {6, 4, 4, 4, '-'}, {6, 0, 7, 0, 'Q'}};
	auto parsed = parseState(serializeState(s));
	ASSERT_TRUE(parsed.has_value());
	ASSERT_EQ(parsed.value().turn, 'B');
	ASSERT_TRUE(std::fabs(parsed.value().whiteTime - 295.5) < 0.01);
	ASSERT_TRUE(std::fabs(parsed.value().blackTime - 300.0) < 0.01);
	ASSERT_EQ(parsed.value().moves.size(), s.moves.size());
	for (size_t i = 0; i < s.moves.size(); i++) {
		ASSERT_TRUE(parsed.value().moves[i] == s.moves[i]);
	}
}

TEST(NetProtocol_StateEmptyMoves) {
	State s;
	s.turn = 'W';
	s.whiteTime = -1;
	s.blackTime = -1;
	auto parsed = parseState(serializeState(s));
	ASSERT_TRUE(parsed.has_value());
	ASSERT_EQ(parsed.value().moves.size(), static_cast<size_t>(0));
}

TEST(NetProtocol_PingPongRoundTrip) {
	auto pi = parsePing(serializePing(123456789LL));
	ASSERT_TRUE(pi.has_value());
	ASSERT_EQ(pi.value(), 123456789LL);
	auto po = parsePong(serializePong(987654321LL));
	ASSERT_TRUE(po.has_value());
	ASSERT_EQ(po.value(), 987654321LL);
}
