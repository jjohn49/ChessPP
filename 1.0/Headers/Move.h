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
    bool isEnPessant;
    bool isCastling;
    bool didCapture;

public:
    Move();
    Move(Piece * piece, char newX, int32_t newY);
    Move(Piece * pieceMoved, char oldX, int32_t oldY, char newX, int32_t newY);
    Move(Piece * pieceMoved, string oldPosition, string newPosition);
    Move(Piece * pieceMoved, char oldX, int32_t oldY, char newX, int32_t newY, bool isEnPessant);
    Move(Piece * pieceMoved, char oldX, int32_t oldY, char newX, int32_t newY, bool isEnPessant, bool isCastling);
    Move(Piece * pieceMoved, char oldX, int32_t oldY, char newX, int32_t newY, bool isEnPessant, bool isCastling, bool didCapture);
    bool operator==(Move move);
    std::string toString();
    pair<char, int32_t> oldPosition();
    pair<char, int32_t> newPosition();
    bool contains(char x, int y);
    Piece * getPiece();
    bool isMoveEnPessant();
    bool isMoveCastling();
    bool didMoveCapture();

};


#endif //CHESSPP_MOVE_H
