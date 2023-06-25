//
// Created by hugh on 6/24/23.
//

#include "../Headers/Board.h"

Board::Board(){
    board = {
            {1,{Piece(),Piece(),Piece(),Piece(),Piece(),Piece(),Piece(),Piece()}},
            {2,{Piece(),Piece(),Piece(),Piece(),Piece(),Piece(),Piece(),Piece()}},
            {3,{Piece(),Piece(),Piece(),Piece(),Piece(),Piece(),Piece(),Piece()}},
            {4,{Piece(),Piece(),Piece(),Piece(),Piece(),Piece(),Piece(),Piece()}},
            {5,{Piece(),Piece(),Piece(),Piece(),Piece(),Piece(),Piece(),Piece()}},
            {6,{Piece(),Piece(),Piece(),Piece(),Piece(),Piece(),Piece(),Piece()}},
            {7,{Piece(),Piece(),Piece(),Piece(),Piece(),Piece(),Piece(),Piece()}},
            {8,{Piece(),Piece(),Piece(),Piece(),Piece(),Piece(),Piece(),Piece()}}
    };
}


string Board::toString() {
    return "";
}

int32_t Board::charToInt(char y) {
    return y - '0';
}

Piece * Board::getPieceAt(int32_t x, char y) {
    return &this->board.at(x)[y];
}

