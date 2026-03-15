//
// Unit tests for Board: initialization, piece access, evaluation,
// move execution, make/unmake, check detection, and deep copy.
//

#include "TestFramework.h"
#include "../src/Headers/Board.h"

// --- Initialization ---

TEST(Board_InitialPiecePositions) {
	Board board{};

	// White back rank
	ASSERT_NOT_NULL(board.getPieceAt(0, 0));
	ASSERT_EQ(board.getPieceAt(0, 0)->getType(), Piece::Rook);
	ASSERT_EQ(board.getPieceAt(0, 0)->getColor(), Piece::White);

	ASSERT_NOT_NULL(board.getPieceAt(0, 4));
	ASSERT_EQ(board.getPieceAt(0, 4)->getType(), Piece::King);
	ASSERT_EQ(board.getPieceAt(0, 4)->getColor(), Piece::White);

	// Black back rank
	ASSERT_NOT_NULL(board.getPieceAt(7, 4));
	ASSERT_EQ(board.getPieceAt(7, 4)->getType(), Piece::King);
	ASSERT_EQ(board.getPieceAt(7, 4)->getColor(), Piece::Black);

	ASSERT_NOT_NULL(board.getPieceAt(7, 3));
	ASSERT_EQ(board.getPieceAt(7, 3)->getType(), Piece::Queen);
	ASSERT_EQ(board.getPieceAt(7, 3)->getColor(), Piece::Black);
}

TEST(Board_InitialPawns) {
	Board board{};

	for (int col = 0; col < 8; col++) {
		// White pawns on row 1
		ASSERT_NOT_NULL(board.getPieceAt(1, col));
		ASSERT_EQ(board.getPieceAt(1, col)->getType(), Piece::Pawn);
		ASSERT_EQ(board.getPieceAt(1, col)->getColor(), Piece::White);

		// Black pawns on row 6
		ASSERT_NOT_NULL(board.getPieceAt(6, col));
		ASSERT_EQ(board.getPieceAt(6, col)->getType(), Piece::Pawn);
		ASSERT_EQ(board.getPieceAt(6, col)->getColor(), Piece::Black);
	}
}

TEST(Board_InitialEmptySquares) {
	Board board{};

	// Rows 2-5 should be empty
	for (int row = 2; row <= 5; row++) {
		for (int col = 0; col < 8; col++) {
			ASSERT_TRUE(board.getPieceAt(row, col) == nullptr);
		}
	}
}

// --- Evaluation ---

TEST(Board_InitialEvaluationIsBalanced) {
	Board board{};
	double eval = board.evaluate();
	// Starting position should be roughly balanced (within positional table noise)
	ASSERT_TRUE(eval > -5.0 && eval < 5.0);
}

TEST(Board_EvaluationChangesAfterCapture) {
	Board board{};
	double initialEval = board.evaluate();

	// Remove a black pawn — evaluation should shift toward white
	board.setPieceAt(6, 0, nullptr);
	double afterEval = board.evaluate();
	ASSERT_GT(afterEval, initialEval);
}

// --- setPieceAt / getPieceAt ---

TEST(Board_SetAndGetPiece) {
	Board board{};

	// Place a white queen on an empty square
	auto piece = board.getPieceAt(0, 3); // white queen
	ASSERT_NOT_NULL(piece);

	board.setPieceAt(4, 4, piece);
	ASSERT_EQ(board.getPieceAt(4, 4), piece);
}

TEST(Board_SetPieceToNull) {
	Board board{};

	ASSERT_NOT_NULL(board.getPieceAt(1, 0)); // white pawn
	board.setPieceAt(1, 0, nullptr);
	ASSERT_TRUE(board.getPieceAt(1, 0) == nullptr);
}

// --- Move Generation ---

TEST(Board_WhiteHasMovesAtStart) {
	Board board{};
	auto moves = board.getAllMovesForColor(Piece::White);
	// White should have 20 moves at start (16 pawn + 4 knight)
	ASSERT_EQ((int)moves.size(), 20);
}

TEST(Board_BlackHasMovesAtStart) {
	Board board{};
	auto moves = board.getAllMovesForColor(Piece::Black);
	ASSERT_EQ((int)moves.size(), 20);
}

// --- Check Detection ---

TEST(Board_NotInCheckAtStart) {
	Board board{};
	ASSERT_FALSE(board.isColorInCheck(Piece::White));
	ASSERT_FALSE(board.isColorInCheck(Piece::Black));
}

TEST(Board_DetectsCheck) {
	Board board{};

	// Clear the path and put a white rook on the black king's file
	// Clear e2 pawn, place rook on e6 aiming at black king e8 (row 7, col 4)
	board.setPieceAt(1, 4, nullptr); // remove white pawn e2
	board.setPieceAt(6, 4, nullptr); // remove black pawn e7

	// Move white queen to e4 to attack along the file
	auto whiteQueen = board.getPieceAt(0, 3);
	board.setPieceAt(0, 3, nullptr);
	board.setPieceAt(4, 4, whiteQueen);
	whiteQueen->setNewPosition(4, 4);

	// The white queen on e4 should give check to the black king on e8
	// (clear path: e4 -> e5 -> e6 -> e7 -> e8, all empty now)
	// But e5 and e6 may have pieces... let's verify
	bool inCheck = board.isColorInCheck(Piece::Black);
	// With queen on e4 and no pawns on e-file, black king should be in check
	ASSERT_TRUE(inCheck);
}

// --- Make/Unmake Search Move ---

TEST(Board_MakeUnmakeRestoresState) {
	Board board{};
	string beforeStr = board.toString();
	double beforeEval = board.evaluate();

	// Get a legal move and apply/undo it
	auto moves = board.getAllMovesForColor(Piece::White);
	ASSERT_TRUE(!moves.empty());

	Move & move = moves[0];
	UndoInfo undo = board.makeSearchMove(move);
	board.unmakeSearchMove(undo);

	string afterStr = board.toString();
	double afterEval = board.evaluate();

	ASSERT_EQ(beforeStr, afterStr);
	ASSERT_EQ(beforeEval, afterEval);
}

TEST(Board_MakeUnmakeMultipleMoves) {
	Board board{};
	string original = board.toString();

	auto whiteMoves = board.getAllMovesForColor(Piece::White);
	ASSERT_TRUE(whiteMoves.size() >= 3);

	// Apply 3 moves and undo them in reverse
	UndoInfo undo1 = board.makeSearchMove(whiteMoves[0]);
	auto blackMoves = board.getAllMovesForColor(Piece::Black);
	ASSERT_TRUE(!blackMoves.empty());
	UndoInfo undo2 = board.makeSearchMove(blackMoves[0]);
	auto whiteMoves2 = board.getAllMovesForColor(Piece::White);
	ASSERT_TRUE(!whiteMoves2.empty());
	UndoInfo undo3 = board.makeSearchMove(whiteMoves2[0]);

	// Undo in reverse
	board.unmakeSearchMove(undo3);
	board.unmakeSearchMove(undo2);
	board.unmakeSearchMove(undo1);

	ASSERT_EQ(board.toString(), original);
}

// --- Deep Copy ---

TEST(Board_DeepCopyIsIndependent) {
	Board board{};
	Board copy = board.deepCopy();

	// Modify the copy
	copy.setPieceAt(1, 0, nullptr);

	// Original should be unaffected
	ASSERT_NOT_NULL(board.getPieceAt(1, 0));
	ASSERT_TRUE(copy.getPieceAt(1, 0) == nullptr);
}

TEST(Board_DeepCopyPreservesEvaluation) {
	Board board{};
	Board copy = board.deepCopy();

	double origEval = board.evaluate();
	double copyEval = copy.evaluate();
	ASSERT_EQ(origEval, copyEval);
}

// --- Utility ---

TEST(Board_InvertRow) {
	Board board{};
	ASSERT_EQ(board.invertRow(0), 7);
	ASSERT_EQ(board.invertRow(7), 0);
	ASSERT_EQ(board.invertRow(3), 4);
}

TEST(Board_IsPositionInBounds) {
	Board board{};
	ASSERT_TRUE(board.isPositionInBounds({0, 0}));
	ASSERT_TRUE(board.isPositionInBounds({7, 7}));
	ASSERT_FALSE(board.isPositionInBounds({-1, 0}));
	ASSERT_FALSE(board.isPositionInBounds({0, 8}));
	ASSERT_FALSE(board.isPositionInBounds({8, 0}));
}

TEST(Board_ToStringNotEmpty) {
	Board board{};
	string str = board.toString();
	ASSERT_TRUE(!str.empty());
	// Should contain king characters
	ASSERT_TRUE(str.find('K') != string::npos); // white king
	ASSERT_TRUE(str.find('k') != string::npos); // black king
}
