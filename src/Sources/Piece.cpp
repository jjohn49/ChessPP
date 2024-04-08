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
    this->hasMoved = false;
}

Piece::Piece(): Piece(-1,-1,NoColor,NoType){}

void Piece::setNewPosition(int row, int col) {
    this->row = row;
    this->col = col;
}

std::string Piece::toString() {
    string ret;
    switch(this->getType()){
        case NoType:
            ret = "NoType";
            break;
        case Pawn:
            ret = "Pawn";
            break;
        case Bishop:
            ret = "Bishop";
            break;
        case Knight:
            ret = "Knight";
            break;
        case Rook:
            ret = "Rook";
            break;
        case Queen:
            ret = "Queen";
            break;
        case King:
            ret = "King";
            break;
    }

    return ret;
};
