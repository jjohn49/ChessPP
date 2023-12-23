//
// Created by John Johnston on 12/4/23.
//

#ifndef CHESSPP_MOVE_H
#define CHESSPP_MOVE_H

#include <iostream>

class Move {
protected:
    std::pair<int,int> oldPosition;
    std::pair<int,int> newPosition;
    bool isEnPessant;

public:
    Move(std::pair<int,int> oldPosition, std::pair<int,int> newPosition){
        this->oldPosition = oldPosition;
        this->newPosition = newPosition;
        this->isEnPessant = false;
    };

    Move(std::pair<int,int> oldPosition, std::pair<int,int> newPosition, bool isEnPessant){
        this->oldPosition = oldPosition;
        this->newPosition = newPosition;
        this->isEnPessant = isEnPessant;
    }

    std::pair<int,int> getOldPosition() {return this->oldPosition;};
    std::pair<int,int> getNewPosition() {return this->newPosition;};
    std::string toString();
    Move getReverseMove();
    bool getIsEnPessant(){return this->isEnPessant;};
    bool operator==(Move move);

};


#endif //CHESSPP_MOVE_H
