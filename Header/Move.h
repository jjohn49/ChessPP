//
// Created by John Johnston on 12/4/23.
//

#ifndef CHESSPP_MOVE_H
#define CHESSPP_MOVE_H


#include "Piece.h"

class Move {
protected:
    Piece * piece;
    std::pair<int,int> oldPosition;
    std::pair<int,int> newPosition;

public:
    Move(Piece * piece, std::pair<int,int> oldPosition, std::pair<int,int> newPosition){
        this->piece = piece;
        this->oldPosition = oldPosition;
        this->newPosition = newPosition;
    }
};


#endif //CHESSPP_MOVE_H
