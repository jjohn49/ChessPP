//
// Created by John Johnston on 7/3/23.
//

#include <utility>

#include "../Headers/Move.h"

Move::Move() {
    Piece p = Piece();
    this->pieceMoved = &p;
    this->oldX = 'i';
    this->oldY = 9;
    this->newX = 'i';
    this->oldY = 9;
}

Move::Move(Piece * pieceMoved, char oldX, int32_t oldY, char newX, int32_t newY) {
    this->pieceMoved = std::move(pieceMoved);
    this->oldX = oldX;
    this->oldY = oldY;
    this->newX = newX;
    this->newY = newY;
    this->isEnPessant = false;
}

Move::Move(Piece * pieceMoved, string oldPosition, string newPosition): Move(std::move(pieceMoved), oldPosition.at(0), oldPosition.at(1), newPosition.at(0), newPosition.at(1)) {
    //Nothing is needed in this code block

}

Move::Move(Piece *pieceMoved, char oldX, int32_t oldY, char newX, int32_t newY, bool isEnPessant) {
    this->pieceMoved = std::move(pieceMoved);
    this->oldX = oldX;
    this->oldY = oldY;
    this->newX = newX;
    this->newY = newY;
    this->isEnPessant = isEnPessant;
}

std::string Move::toString() {
    return this->pieceMoved->color + this->pieceMoved->name + ": " + this->oldX + to_string(this->oldY) + " -> " + this->newX +
            to_string(this->newY);
}

pair<char, int32_t> Move::oldPosition() {
    return make_pair(this->oldX,(char)this->oldY);
}

pair<char, int32_t> Move::newPosition() {
    return make_pair(this->newX,(char)this->newY);
}

Piece *Move::getPiece() {
    return pieceMoved;
}

Move::Move(Piece * piece, char newX, int32_t newY): Move(piece, piece->x, piece->y, newX, newY) {
//need nothing here
}

bool Move::contains(char x, int y) {
    return this->newX == x && this->newY == y;
}

bool Move::operator==(Move move) {
    return this->pieceMoved == move.pieceMoved && this->oldX == move.oldX && this->oldY == move.oldY && this->newX == move.newX && this->newY == move.newY;
}

bool Move::isMoveEnPessant() {
    return this->isEnPessant;
}




