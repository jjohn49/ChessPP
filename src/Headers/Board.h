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
#include "Move.h"

using namespace std;

class Board {

protected:
    vector<vector<shared_ptr<Piece>>> board;
    vector<Pawn> pawns;
public:

    Board();

    shared_ptr<Piece> getPieceAt(int row, int col);
    shared_ptr<Piece> getPieceAt(pair<int, int> position);

    void setPieceAt(int row, int col, std::shared_ptr<Piece> pieceDragging);
    void setPieceAt(pair<int,int> position, std::shared_ptr<Piece> pieceDragging);

    void movePiece(Move move);

    void print();
};


#endif //CHESSPP_BOARD_H
