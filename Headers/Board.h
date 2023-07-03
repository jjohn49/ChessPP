//
// Created by hugh on 6/24/23.
//

#ifndef CHESSPP_BOARD_H
#define CHESSPP_BOARD_H

#include "string"
#include <unordered_map>
#include "Piece.h"
#include "array"
using namespace std;

class Board {
    unordered_map<int32_t , array<optional<Piece>,8>>  board;
public:
    Board();
    //~Board();
    optional<Piece> getPieceAt(int32_t x, char y);
    void printBoard();
};


#endif //CHESSPP_BOARD_H
