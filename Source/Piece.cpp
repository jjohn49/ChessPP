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
}

Piece::Piece(Piece::Type type, Piece::Color color, int row, int col) {
    this->type = type;
    this->color = color;
    this->row = row;
    this->col = col;
}

std::vector<Move> Piece::getMoves(std::shared_ptr<Piece> board[8][8]) {
    return {std::enable_shared_from_this<Piece>}
}
