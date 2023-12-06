//
// Created by John Johnston on 12/4/23.
//

#include "../Header/Piece.h"
#include "../Header/Move.h"

Piece::Piece() {
    this->type = Typeless;
    this->color = Colorless;
    this->row = -1;
    this->col = -1;
    this->hasMoved = false;
}

Piece::Piece(Piece::Type type, Piece::Color color, int row, int col) {
    this->type = type;
    this->color = color;
    this->row = row;
    this->col = col;
    this->hasMoved = false;
}

