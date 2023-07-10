//
// Created by John Johnston on 7/3/23.
//

#ifndef CHESSPP_MOVE_H
#define CHESSPP_MOVE_H

#include <iostream>
#include "./Pieces/Piece.h"


class Move {
    Piece * pieceMoved;
    char oldX;
    int32_t oldY;
    char newX;
    int32_t newY;

public:
    Move();
    Move(Piece * pieceMoved, char oldX, int32_t oldY, char newX, int32_t newY);
    Move(Piece * pieceMoved, string oldPosition, string newPosition);
    std::string toString();
    pair<char, int32_t> oldPosition();
    pair<char, int32_t> newPosition();
    Piece * getPiece();

};


#endif //CHESSPP_MOVE_H
