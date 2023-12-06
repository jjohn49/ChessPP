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

public:
    Move(std::pair<int,int> oldPosition, std::pair<int,int> newPosition){
        this->oldPosition = oldPosition;
        this->newPosition = newPosition;
    }

    std::string toString();
};


#endif //CHESSPP_MOVE_H
