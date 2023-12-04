//
// Created by John Johnston on 12/4/23.
//

#ifndef CHESSPP_PIECE_H
#define CHESSPP_PIECE_H
#include <iostream>

class Move;

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
    Piece(Type type, Color color);
    std::string toString(){
        return this->type + "" + this->color;
    };

    std::vector<Move> getMoves(std::vector<std::vector<Piece *>> *);

protected:
    Type type;
    Color color;

};


#endif //CHESSPP_PIECE_H
