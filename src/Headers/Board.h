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

using namespace std;

class Board {

protected:
    vector<vector<shared_ptr<Piece>>> board;
    vector<shared_ptr<Pawn>> pawns;
    vector<shared_ptr<King>> kings;
    vector<shared_ptr<Rook>> rooks;
    vector<shared_ptr<Bishop>> bishops;
    vector<shared_ptr<Knight>> knights;
    vector<Move> moveHistory;

public:

    Board();

    shared_ptr<Piece> getPieceAt(int row, int col);
    shared_ptr<Piece> getPieceAt(pair<int, int> position);

    void setPieceAt(int row, int col, std::shared_ptr<Piece> pieceDragging);
    void setPieceAt(pair<int,int> position, std::shared_ptr<Piece> pieceDragging);

    void movePiece(Move & move);

    std::pair<int,int> getColorsKingPosition(Piece::Color color);

    bool isColorInCheck(Piece::Color color);
    std::vector<Move> getAllMovesForColor(Piece::Color color);

    Move getLastMove();
    bool isFirstMove();
    shared_ptr<Rook> getRook(int val);
    bool isPositionInOppMoves(pair<int,int> position, Piece::Color oppColor);
    bool isPositionsInOppMoves(vector<pair<int,int>> positions, Piece::Color oppColor);
    void print();
    bool isPositionInBounds(pair<int,int> position);
};


#endif //CHESSPP_BOARD_H
