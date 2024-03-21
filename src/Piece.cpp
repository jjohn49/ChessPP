//
// Created by hugh on 3/21/24.
//

#include "Piece.h"



Piece::Piece(int row, int col, Piece::Color color, Piece::Type type) {
    this->row = row;
    this->col = col;
    this->color = color;
    this->type = type;
}

Piece::Piece(): Piece(-1,-1,NoColor,NoType){};
