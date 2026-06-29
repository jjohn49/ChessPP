//
// Regression tests for check detection during interactive (drag) move validation.
//
// The drag UI lifts the moving piece off the board grid before validating the
// move. When that piece is the KING, Board::deepCopy() (used by Player::isInCheck)
// drops the lifted king from its `kings` cache, so king-position lookup must fall
// back to the board grid rather than trusting a stale cache index.
//

#include "TestFramework.h"
#include "../src/Headers/Board.h"
#include "../src/Headers/Player.h"
#include <memory>

// A king lifted off the grid (as during a drag) must still be detected as in
// check when the candidate destination remains attacked.
TEST(Check_LiftedKingStillDetectsCheck) {
	Board board{};
	// Relocate the (registered) black a8 rook to e5 and open the e-file toward
	// the white king. Using an existing rook keeps it in board.rooks so deepCopy
	// preserves it.
	std::shared_ptr<Piece> br = board.getPieceAt(7, 0);
	ASSERT_EQ(br->getType(), Piece::Rook);
	board.setPieceAt(7, 0, nullptr);
	board.setPieceAt(1, 4, nullptr);   // remove white e2 pawn
	board.setPieceAt(6, 4, nullptr);   // remove black e7 pawn
	board.setPieceAt(4, 4, br);
	br->setNewPosition(4, 4);          // keep internal position consistent with the grid

	std::shared_ptr<Piece> wk = board.getPieceAt(0, 4);   // white king
	ASSERT_EQ(wk->getType(), Piece::King);
	ASSERT_TRUE(board.isColorInCheck(Piece::White));      // rook checks down the open e-file

	Player white{Piece::White, &board};

	// Simulate the drag UI lifting the king off the grid before validation.
	board.setPieceAt(0, 4, nullptr);

	// Staying on the e-file (e2) keeps the king in check -> must be reported true.
	ASSERT_TRUE(white.isInCheck(wk, std::make_pair(1, 4)));
	// c3 is off every black piece's lines -> safe (sanity that it isn't always true).
	ASSERT_FALSE(white.isInCheck(wk, std::make_pair(2, 2)));
}

// getColorsKingPosition must return the correct color's king even when the kings
// cache is stale (a king dropped from a deep-copied board, as happens when the
// king is lifted off the grid during drag validation).
TEST(Check_KingPositionFallsBackToGridWhenCacheStale) {
	Board board{};
	std::shared_ptr<Piece> wk = board.getPieceAt(0, 4);
	board.setPieceAt(0, 4, nullptr);          // lift the king (as the drag UI does)

	Board copy = board.deepCopy();            // copy's kings cache now lacks the white king
	copy.setPieceAt(3, 4, wk);                 // place the king on the copy
	wk->setNewPosition(3, 4);

	std::pair<int,int> pos = copy.getColorsKingPosition(Piece::White);
	ASSERT_EQ(pos.first, 3);
	ASSERT_EQ(pos.second, 4);
}
