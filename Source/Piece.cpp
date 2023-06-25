//
// Created by hugh on 6/24/23.
//

#include <utility>

#include "../Headers/Piece.h"

Piece::Piece() {
    this->name = "Piece";
    this->x = 0;
    this->y = '\0';
}

Piece::Piece(string name, int32_t x, char y) {
    this->name=std::move(name);
    this->x=x;
    this->y=y;
}

void Piece::setX(char x) {
    this->x=x;
}

void Piece::setY(int32_t y) {
    this->y=y;
}

string Piece::toString() {
    return this->name + " at location " + this->x + to_string(this->y);
}

