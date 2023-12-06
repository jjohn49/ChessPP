//
// Created by John Johnston on 12/4/23.
//

#ifndef CHESSPP_PIECE_H
#define CHESSPP_PIECE_H
#include <iostream>
#include <vector>
#include <unordered_map>
#include <memory>
#include "Move.h"


class Piece {

public:

    enum Type{
        Typeless = 0,
        Pawn = 1,
        Bishop = 3,
        Knight = 3,
        Rook = 5,
        Queen = 7,
        King = 10
    };

    enum Color{
        Colorless,
        White,
        Black
    };

    Piece();
    Piece(Type type, Color color, int row, int col);
    std::string toString(){
        return this->colorToString.at(this->color) + " " + this->typeToString.at(this->type) + char(this->row + 48) + char(this->col+48);
    };

    virtual std::vector<Move> getMoves(std::shared_ptr<Piece> board[8][8]) { return {}; };

    std::pair<int,int> getPosition(){ return std::make_pair(this->row, this->col); };

    void setNewPosition(std::pair<int,int> newPos) {this->row = newPos.first; this->col = newPos.second;};

protected:
    Type type;
    Color color;
    int row;
    int col;
    bool hasMoved;
    std::unordered_map<Type, std::string> typeToString{
            {Typeless,"Typeless"},
            {Pawn,"Pawn"},
            {Bishop,"Bishop"},
            {Rook,"Rook"},
            {Knight,"Knight"},
            {Queen,"Queen"},
            {King,"King"}
    };

    std::unordered_map<Color,std::string> colorToString{
            {Colorless,"Colorless"},
            {White,"White"},
            {Black,"Black"}
    };

};


#endif //CHESSPP_PIECE_H
