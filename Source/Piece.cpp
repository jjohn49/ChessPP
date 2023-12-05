//
// Created by John Johnston on 12/4/23.
//

#include "../Header/Piece.h"
#include "../Header/Move.h"

Piece::Piece() {
    this->type = Typeless;
    this->color = Colorless;
}

Piece::Piece(Piece::Type type, Piece::Color color) {
    this->type = type;
    this->color = color;
}

std::vector<Move> Piece::getMoves(std::shared_ptr<Piece> board[8][8]) {
    return {std::enable_shared_from_this<Piece>}
}
