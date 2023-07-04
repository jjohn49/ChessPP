//
// Created by John Johnston on 7/3/23.
//

#include <utility>

#include "../Headers/Move.h"

Move::Move() {
    this->pieceMoved = Piece();
    this->oldX = 'i';
    this->oldY = 9;
    this->newX = 'i';
    this->oldY = 9;
}

Move::Move(Piece pieceMoved, char oldX, int32_t oldY, char newX, int32_t newY) {
    this->pieceMoved = std::move(pieceMoved);
    this->oldX = oldX;
    this->oldY = oldY;
    this->newX = newX;
    this->newY = newY;
}

Move::Move(Piece pieceMoved, string oldPosition, string newPosition): Move(std::move(pieceMoved), oldPosition.at(0), oldPosition.at(1), newPosition.at(0), newPosition.at(1)) {
    //Nothing is needed in this code block
}

std::string Move::toString() {
    return this->pieceMoved.color + this->pieceMoved.name + ": " + this->oldX + (char)this->oldY + " -> " + this->newX +
            (char)this->newY;
}

std::string Move::oldPosition() {
    return std::string() + this->oldX + (char)this->oldY;
}

std::string Move::newPosition() {
    return std::string() + this->newX + (char)this->newY;
}


