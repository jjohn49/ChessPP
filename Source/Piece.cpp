//
// Created by John Johnston on 12/4/23.
//

#include "../Header/Piece.h"

Piece::Piece() {
    this->type = Typeless;
    this->color = Colorless;
}

Piece::Piece(Piece::Type type, Piece::Color color) {
    this->type = type;
    this->color = color;
}
