//
// Created by John Johnston on 7/3/23.
//

#ifndef CHESSPP_MOVE_H
#define CHESSPP_MOVE_H

#include <iostream>
#include "Pieces/Piece.h"

class Move {
    Piece pieceMoved;
    char oldX;
    int32_t oldY;
    char newX;
    int32_t newY;

public:
    Move(Piece pieceMoved, char oldX, int32_t oldY, char newX, int32_t newY);
    Move(Piece pieceMoved, char oldPosition[2], char newPosition[2]);
    std::string toString();
    std::string oldPosition();
    std::string newPosition();
};


#endif //CHESSPP_MOVE_H
