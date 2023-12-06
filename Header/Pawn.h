//
// Created by John Johnston on 12/4/23.
//

#ifndef CHESSPP_PAWN_H
#define CHESSPP_PAWN_H


#include "Piece.h"

class Pawn: public Piece {
public:
    Pawn(): Piece(Piece::Type::Pawn, Piece::Color::Colorless){};
    Pawn(Piece::Color color) : Piece(Piece::Type::Pawn, color){};
};


#endif //CHESSPP_PAWN_H
