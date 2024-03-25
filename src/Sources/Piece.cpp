//
// Created by hugh on 3/21/24.
//

#include "../Headers/Piece.h"
#include <sstream>



Piece::Piece(int row, int col, Piece::Color color, Piece::Type type) {
    this->row = row;
    this->col = col;
    this->color = color;
    this->type = type;
}

Piece::Piece(): Piece(-1,-1,NoColor,NoType){}

void Piece::setNewPosition(int row, int col) {
    this->row = row;
    this->col = col;
}

std::string Piece::toString() {
    std::ostringstream strout;
    strout << color << type  << row << col;
    return strout.str();
};
