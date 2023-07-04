//
// Created by John Johnston on 7/3/23.
//

#ifndef CHESSPP_PAWN_H
#define CHESSPP_PAWN_H

#include "./Piece.h"
#include "../Move.h"


class Pawn: public Piece {
    Pawn(char color, char x, int32_t y);
};


#endif //CHESSPP_PAWN_H
