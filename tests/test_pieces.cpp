///
// Unit tests for individual piece move generation.
// Verifies each piece type generates the correct moves
// from known positions on an otherwise empty or starting board.
//

#include "TestFramework.h"
#include "../src/Headers/Board.h"

// Helper: create an empty board (clear all pieces from starting position)
static Board makeEmptyBoard() {
	Board board{};
	for (int r = 0; r < 8; r++) {
		for (int c = 0; c < 8; c++) {
			board.setPieceAt(r, c, nullptr);
		}
	}
	return board;
}

// --- Pawn ---

TEST(Pawn_InitialDoubleMove) {
	Board board{};
	// White pawn at (1, 0) should have 2 moves: single and double push
	auto pawn = board.getPieceAt(1, 0);
	ASSERT_NOT_NULL(pawn);
	ASSERT_EQ(pawn->getType(), Piece::Pawn);

	auto moves = pawn->getMoves(&board);
	ASSERT_EQ((int)moves.size(), 2);
}

TEST(Pawn_BlockedByPiece) {
	Board board{};
	// Place a piece in front of a white pawn
	auto blocker = board.getPieceAt(0, 1); // white knight
	board.setPieceAt(2, 0, blocker);
	blocker->setNewPosition(2, 0);

	auto pawn = board.getPieceAt(1, 0);
	auto moves = pawn->getMoves(&board);
	// Pawn should have no forward moves
	ASSERT_EQ((int)moves.size(), 0);
}

TEST(Pawn_CanCaptureDiagonally) {
	Board board{};
	// Place a black piece diagonally in front of white pawn at (1, 3)
	auto blackPawn = board.getPieceAt(6, 0); // a black pawn
	board.setPieceAt(2, 4, blackPawn);
	blackPawn->setNewPosition(2, 4);

	auto whitePawn = board.getPieceAt(1, 3);
	auto moves = whitePawn->getMoves(&board);
	// Should have: single push, double push, and diagonal capture
	ASSERT_EQ((int)moves.size(), 3);
}

// --- Knight ---

TEST(Knight_InitialMoves) {
	Board board{};
	// White knight at (0, 1) should have 2 moves from starting position
	auto knight = board.getPieceAt(0, 1);
	ASSERT_NOT_NULL(knight);
	ASSERT_EQ(knight->getType(), Piece::Knight);

	auto moves = knight->getMoves(&board);
	ASSERT_EQ((int)moves.size(), 2);
}

TEST(Knight_CenterHas8Moves) {
	Board board = makeEmptyBoard();
	auto knight = std::make_shared<Knight>(4, 4, Piece::White);
	board.setPieceAt(4, 4, knight);

	auto moves = knight->getMoves(&board);
	ASSERT_EQ((int)moves.size(), 8);
}

TEST(Knight_CornerHas2Moves) {
	Board board = makeEmptyBoard();
	auto knight = std::make_shared<Knight>(0, 0, Piece::White);
	board.setPieceAt(0, 0, knight);

	auto moves = knight->getMoves(&board);
	ASSERT_EQ((int)moves.size(), 2);
}

// --- Bishop ---

TEST(Bishop_EmptyBoardCenter) {
	Board board = makeEmptyBoard();
	auto bishop = std::make_shared<Bishop>(4, 4, Piece::White);
	board.setPieceAt(4, 4, bishop);

	auto moves = bishop->getMoves(&board);
	// Center bishop should have 13 diagonal squares
	ASSERT_EQ((int)moves.size(), 13);
}

TEST(Bishop_BlockedByFriendly) {
	Board board = makeEmptyBoard();
	auto bishop = std::make_shared<Bishop>(0, 0, Piece::White);
	board.setPieceAt(0, 0, bishop);

	// Place a friendly piece blocking the diagonal
	auto blocker = std::make_shared<Pawn>(2, 2, Piece::White);
	board.setPieceAt(2, 2, blocker);

	auto moves = bishop->getMoves(&board);
	// Can only reach (1,1) before being blocked
	ASSERT_EQ((int)moves.size(), 1);
}

// --- Rook ---

TEST(Rook_EmptyBoardCenter) {
	Board board = makeEmptyBoard();
	auto rook = std::make_shared<Rook>(4, 4, Piece::White);
	board.setPieceAt(4, 4, rook);

	auto moves = rook->getMoves(&board);
	// Center rook should have 14 orthogonal squares
	ASSERT_EQ((int)moves.size(), 14);
}

TEST(Rook_CanCaptureEnemy) {
	Board board = makeEmptyBoard();
	auto rook = std::make_shared<Rook>(4, 4, Piece::White);
	board.setPieceAt(4, 4, rook);

	auto enemy = std::make_shared<Pawn>(4, 6, Piece::Black);
	board.setPieceAt(4, 6, enemy);

	auto moves = rook->getMoves(&board);
	// Should be able to capture the enemy pawn but not go past it
	bool canCapture{false};
	bool goesPast{false};
	for (auto & m : moves) {
		if (m.getNewPosition() == make_pair(4, 6)) canCapture = true;
		if (m.getNewPosition() == make_pair(4, 7)) goesPast = true;
	}
	ASSERT_TRUE(canCapture);
	ASSERT_FALSE(goesPast);
}

// --- Queen ---

TEST(Queen_EmptyBoardCenter) {
	Board board = makeEmptyBoard();
	auto queen = std::make_shared<Queen>(4, 4, Piece::White);
	board.setPieceAt(4, 4, queen);

	auto moves = queen->getMoves(&board);
	// Queen = rook (14) + bishop (13) = 27 from center
	ASSERT_EQ((int)moves.size(), 27);
}

// --- King ---

TEST(King_CenterHas8Moves) {
	Board board = makeEmptyBoard();
	auto king = std::make_shared<King>(Piece::White);
	king->setNewPosition(4, 4);
	board.setPieceAt(4, 4, king);

	board.generatingAttacks = true; // skip castling checks
	auto moves = king->getMoves(&board);
	board.generatingAttacks = false;

	ASSERT_EQ((int)moves.size(), 8);
}

TEST(King_CornerHas3Moves) {
	Board board = makeEmptyBoard();
	auto king = std::make_shared<King>(Piece::White);
	king->setNewPosition(0, 0);
	board.setPieceAt(0, 0, king);

	board.generatingAttacks = true;
	auto moves = king->getMoves(&board);
	board.generatingAttacks = false;

	ASSERT_EQ((int)moves.size(), 3);
}
