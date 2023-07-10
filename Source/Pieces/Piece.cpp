//
// Created by hugh on 6/24/23.
//

#include <utility>

#include "../../Headers/Pieces/Piece.h"

Piece::Piece() {
    this->name = "Piece";
    this->x = 'i';
    this->y = 0;
    this->color = 'w';
    this->hasMoved = false;
}

Piece::Piece(string name, char color, char x, int32_t y) {
    this->name=std::move(name);
    this->x=x;
    this->y=y;
    this->color=color;
    this->hasMoved=false;
}

void Piece::setX(char x) {
    this->x=x;
}

void Piece::setY(int32_t y) {
    this->y=y;
}

string Piece::toString() {
    return this->color + this->name + " at location " + this->x + to_string(this->y);
}

string Piece::toShortString() {
    return this->color + this->name;
}

bool Piece::operator==(Piece piece) {
    return this->name==piece.name && this->color==piece.color && this->x==piece.x && this->y==piece.y;
}

bool Piece::operator==(optional<Piece> p) {
    if(!p.has_value()){
        return false;
    }
    return *this == p.value();
}

bool Piece::isSameColor(Piece * piece) {
    return this->color == piece->color;
}




