//
// Unit tests for Bot: move selection, search algorithms,
// and move ordering.
//

#include "TestFramework.h"
#include "../src/Headers/Board.h"
#include "../src/Headers/Bot.h"

// --- Move Sorting ---

TEST(Bot_AllMovesSortedNotEmpty) {
	Board board{};
	Bot bot{Easy, Piece::White, &board};
	auto sorted = bot.getAllMovesSorted();
	ASSERT_TRUE(!sorted.empty());
}

TEST(Bot_AllMovesSortedHasCorrectCount) {
	Board board{};
	Bot bot{Easy, Piece::White, &board};
	auto sorted = bot.getAllMovesSorted();
	// Should match total legal moves for white (20 at start)
	ASSERT_EQ((int)sorted.size(), 20);
}

// --- MinMax Search ---

TEST(Bot_MinMaxReturnsValidMove) {
	Board board{};
	Bot bot{Easy, Piece::White, &board};
	Move move = bot.getMinMaxMove(1);

	// The move should have a valid moving piece
	ASSERT_NOT_NULL(move.getMovingPiece());
	ASSERT_EQ(move.getMovingPiece()->getColor(), Piece::White);

	// Old and new positions should be different
	ASSERT_TRUE(move.getOldPosition() != move.getNewPosition());
}

TEST(Bot_MinMaxDepth2ReturnsValidMove) {
	Board board{};
	Bot bot{Intermediate, Piece::White, &board};
	Move move = bot.getMinMaxMove(2);

	ASSERT_NOT_NULL(move.getMovingPiece());
	ASSERT_EQ(move.getMovingPiece()->getColor(), Piece::White);
}

TEST(Bot_MinMaxBlackReturnsValidMove) {
	Board board{};
	Bot bot{Easy, Piece::Black, &board};
	Move move = bot.getMinMaxMove(1);

	ASSERT_NOT_NULL(move.getMovingPiece());
	ASSERT_EQ(move.getMovingPiece()->getColor(), Piece::Black);
}

// --- Deeper search ---

TEST(Bot_SearchDepth1ReturnsValidMove) {
	Board board{};
	Bot bot{Intermediate, Piece::White, &board};
	Move move = bot.getMinMaxMove(1);

	ASSERT_NOT_NULL(move.getMovingPiece());
	ASSERT_EQ(move.getMovingPiece()->getColor(), Piece::White);
}

TEST(Bot_SearchDepth2BlackReturnsValidMove) {
	Board board{};
	Bot bot{Intermediate, Piece::Black, &board};
	Move move = bot.getMinMaxMove(2);

	ASSERT_NOT_NULL(move.getMovingPiece());
	ASSERT_EQ(move.getMovingPiece()->getColor(), Piece::Black);
}

// --- Alpha-Beta correctness ---

TEST(Bot_MinMaxDoesNotCorruptBoard) {
	Board board{};
	string before = board.toString();

	Bot bot{Easy, Piece::White, &board};
	bot.getMinMaxMove(2);

	// Board should be unchanged after search
	string after = board.toString();
	ASSERT_EQ(before, after);
}

TEST(Bot_Depth3DoesNotCorruptBoard) {
	Board board{};
	string before = board.toString();

	Bot bot{Intermediate, Piece::White, &board};
	bot.getMinMaxMove(3);

	string after = board.toString();
	ASSERT_EQ(before, after);
}

// --- Bot captures free pieces ---

TEST(Bot_CapturesHangingPiece) {
	Board board{};

	// From starting position, place a black rook on e4 where white queen can capture it.
	// Clear the path: remove white e2 pawn.
	board.setPieceAt(1, 4, nullptr);

	// Place a black rook on e3 (undefended, white queen on d1 can capture)
	auto blackRook = board.getPieceAt(7, 0); // grab a black rook
	board.setPieceAt(7, 0, nullptr);
	board.setPieceAt(2, 4, blackRook);
	blackRook->setNewPosition(2, 4);

	Bot bot{Hard, Piece::White, &board};
	Move move = bot.getMinMaxMove(2);

	// The bot should capture the hanging rook (highest-value undefended piece)
	ASSERT_NOT_NULL(move.getCapturedPiece());
}

// Regression: bot (black) used to answer 1.b4 with ...c5, hanging the pawn to bxc5.
// Coordinates: row 0 = white back rank, row 1 = white pawns, row 6 = black pawns.
// b2=(1,1), b4=(3,1), c7=(6,2), c5=(4,2).
TEST(Bot_DoesNotHangPawnAfterB4) {
	Board board{};

	// Simulate white having played b2-b4.
	auto whiteBPawn = board.getPieceAt(1, 1);
	board.setPieceAt(1, 1, nullptr);
	board.setPieceAt(3, 1, whiteBPawn);
	whiteBPawn->setNewPosition(3, 1);
	whiteBPawn->setHasMoved(true);

	// Black to move at a realistic difficulty depth.
	Bot bot{Hard, Piece::Black, &board};
	Move move = bot.getMinMaxMove(4);

	ASSERT_NOT_NULL(move.getMovingPiece());
	ASSERT_EQ(move.getMovingPiece()->getColor(), Piece::Black);

	// It must NOT push the c-pawn from c7 to c5 (which bxc5 wins for free).
	bool isC7toC5 = move.getOldPosition() == make_pair(6, 2)
	             && move.getNewPosition() == make_pair(4, 2);
	ASSERT_TRUE(!isC7toC5);
}

// --- Difficulty levels produce valid moves ---

TEST(Bot_RandomMoveIsValid) {
	Board board{};
	Bot bot{Random, Piece::White, &board};
	Move move = bot.getRandomMove();
	ASSERT_NOT_NULL(move.getMovingPiece());
}

TEST(Bot_SuperEasyMoveIsValid) {
	Board board{};
	Bot bot{SuperEasy, Piece::White, &board};
	Move move = bot.getSuperEasyMove();
	ASSERT_NOT_NULL(move.getMovingPiece());
}
