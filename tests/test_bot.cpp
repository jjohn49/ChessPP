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

// --- NegaMax Search ---

TEST(Bot_NegaMaxReturnsValidMove) {
	Board board{};
	Bot bot{Intermediate, Piece::White, &board};
	Move move = bot.getNegaMaxMove(1);

	ASSERT_NOT_NULL(move.getMovingPiece());
	ASSERT_EQ(move.getMovingPiece()->getColor(), Piece::White);
}

TEST(Bot_NegaMaxDepth2ReturnsValidMove) {
	Board board{};
	Bot bot{Intermediate, Piece::Black, &board};
	Move move = bot.getNegaMaxMove(2);

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

TEST(Bot_NegaMaxDoesNotCorruptBoard) {
	Board board{};
	string before = board.toString();

	Bot bot{Intermediate, Piece::White, &board};
	bot.getNegaMaxMove(2);

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
