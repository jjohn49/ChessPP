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

using namespace std;

class Board {

protected:
    vector<vector<shared_ptr<Piece>>> board;
    vector<Pawn> pawns;
public:

    Board();

    Piece * getPieceAt(pair<int,int> position);
    Piece * getPieceAt(int row, int col);
    void print();
};


#endif //CHESSPP_BOARD_H
