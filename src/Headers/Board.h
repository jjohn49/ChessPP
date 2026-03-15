//
// Created by hugh on 3/21/24.
//

#ifndef CHESSPP_BOARD_H
#define CHESSPP_BOARD_H

#include <iostream>
#include <vector>
#include <memory>
#include "Piece.h"
#include "Pawn.h"
#include "King.h"
#include "Move.h"
#include "Rook.h"
#include "Bishop.h"
#include "Knight.h"
#include "Queen.h"

using namespace std;

// Stores all state needed to undo a search move via unmakeSearchMove().
// Captures piece positions, hasMoved flags, and special move state
// (castling rook, en passant capture, pawn promotion) so the board
// can be fully restored after recursive search.
struct UndoInfo {
	pair<int,int> oldPos;
	pair<int,int> newPos;
	shared_ptr<Piece> capturedPiece;
	bool movingPieceHadMoved;
	pair<int,int> movingPieceOldPosition;
	// Castling
	bool wasCastle = false;
	pair<int,int> rookOldPos;
	pair<int,int> rookNewPos;
	shared_ptr<Piece> rookPiece;
	bool rookHadMoved = false;
	// En passant
	bool wasEnPassant = false;
	pair<int,int> enPassantCapturePos;
	shared_ptr<Piece> enPassantCapturedPiece;
	// Pawn promotion
	bool wasPromotion = false;
	shared_ptr<Piece> originalPawn;
	shared_ptr<Piece> promotedQueen;
};

// Represents the 8x8 chess board.
// Owns all piece objects via shared_ptr and provides move execution,
// move generation, check detection, and static evaluation.
class Board {

protected:
	vector<vector<shared_ptr<Piece>>> board;
	vector<shared_ptr<Pawn>> pawns;
	vector<shared_ptr<King>> kings;
	vector<shared_ptr<Rook>> rooks;
	vector<shared_ptr<Bishop>> bishops;
	vector<shared_ptr<Knight>> knights;
	vector<shared_ptr<Queen>> queens;
	vector<Move> moveHistory;

public:
	// Set to true during attack map generation to prevent infinite recursion
	// (e.g. castling legality checks generating opponent moves which check castling again)
	bool generatingAttacks = false;

	Board();

	// Piece access
	shared_ptr<Piece> getPieceAt(int row, int col);
	shared_ptr<Piece> getPieceAt(pair<int, int> position);
	void setPieceAt(int row, int col, std::shared_ptr<Piece> piece);
	void setPieceAt(pair<int,int> position, std::shared_ptr<Piece> piece);

	// Game move execution (permanent, updates moveHistory)
	void movePiece(Move & move);

	// Search move execution (reversible, does not update moveHistory)
	UndoInfo makeSearchMove(Move & move);
	void unmakeSearchMove(UndoInfo & undo);

	// King and check detection
	std::pair<int,int> getColorsKingPosition(Piece::Color color);
	bool isColorInCheck(Piece::Color color);
	bool isPositionsInOppMoves(vector<pair<int,int>> positions, Piece::Color oppColor);

	// Move generation
	std::vector<Move> getAllMovesForColor(Piece::Color color);

	// Move history
	Move getLastMove();
	bool isFirstMove();

	// Piece access helpers
	shared_ptr<Rook> getRook(int val);
	shared_ptr<Piece> addPiece(Piece::Type type, Piece::Color color, pair<int,int> position);

	// Evaluation (positive = white advantage)
	double evaluate();

	// Utilities
	bool isPositionInBounds(pair<int,int> position);
	int invertRow(int row);
	void print();
	string toString();
	Board deepCopy();
};

#endif //CHESSPP_BOARD_H
