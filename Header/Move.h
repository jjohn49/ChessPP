//
// Created by John Johnston on 12/4/23.
//

#ifndef CHESSPP_MOVE_H
#define CHESSPP_MOVE_H


#include "Piece.h"

class Move {
protected:
    std::shared_ptr<Piece> piece;
    std::pair<int,int> oldPosition;
    std::pair<int,int> newPosition;

public:
    Move(std::shared_ptr<Piece> piece, std::pair<int,int> oldPosition, std::pair<int,int> newPosition){
        this->piece = std::move(piece);
        this->oldPosition = oldPosition;
        this->newPosition = newPosition;
    }

    std::string toString(){
      return piece->toString() + ": " + char(oldPosition.first + 97) + "->" + char(oldPosition.second);
    };
};


#endif //CHESSPP_MOVE_H
