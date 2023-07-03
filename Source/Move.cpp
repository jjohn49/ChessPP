//
// Created by John Johnston on 7/3/23.
//

#include <utility>

#include "../Headers/Move.h"

std::string Move::toString() {
    return this->pieceMoved.color + this->pieceMoved.name + ": " + this->oldX + to_string(this->oldY) + " -> " + this->newX +
            to_string(this->newY);
}

std::string Move::oldPosition() {
    return std::string();
}

std::string Move::newPosition() {
    return std::string();
}

Move::Move(Piece pieceMoved, char oldX, int32_t oldY, char newX, int32_t newY) {
    this->pieceMoved = std::move(pieceMoved);
    this->oldX = oldX;
    this->oldY = oldY;
    this->newX = newX;
    this->newY = newY;
}

Move::Move(Piece pieceMoved, char *oldPosition, char *newPosition): Move(std::move(pieceMoved), oldPosition[0], oldPosition[1], newPosition[0], newPosition[1]) {
    //Nothing is needed in this code block
}
