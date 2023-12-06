//
// Created by John Johnston on 12/4/23.
//

#include "../Header/Move.h"

std::string Move::toString(){
    std::string ret{""};

    ret.push_back(char(this->oldPosition.first + 48));
    ret.push_back(char(this->oldPosition.second + 48));
    ret += " -> ";
    ret.push_back(char(this->newPosition.first + 48));
    ret.push_back(char(this->newPosition.second + 48));
    ret += "\n";
    return ret;
}
