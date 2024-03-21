//
// Created by hugh on 3/21/24.
//

#ifndef CHESSPP_BOARD_H
#define CHESSPP_BOARD_H

#include <iostream>
#include <vector>
#include <memory>
#include "Piece.h"

using namespace std;

class Board {

protected:
    vector<vector<shared_ptr<Piece>>> board;
    vector<Piece> pieces;
public:

    Board();

    Piece * getPieceAt(pair<int,int> position);
    Piece * getPieceAt(int row, int col);
};


#endif //CHESSPP_BOARD_H
