//
// Created by hugh on 3/21/24.
//

#ifndef CHESSPP_PIECE_H
#define CHESSPP_PIECE_H

#include <iostream>
#include "Move.h"
#include <vector>

using namespace std;

class Board;
class Piece {
public:
    enum Type{
        NoType,
        Pawn,
        Bishop,
        Knight,
        Rook,
        Queen,
        King
    };

    enum Color{
        NoColor,
        White,
        Black
    };

protected:
    int row;
    int col;
    bool hasMoved;
    Color color;
    Type type;
public:


    Piece();
    Piece(int row, int col, Color color, Type type);

    pair<int, int> getPosition(){return make_pair(row, col);};
    Color getColor(){return color;};
    Type getType(){return type;};
    bool getHasMoved(){return hasMoved;};

    virtual vector<Move> getMoves(Board * board){return  {};};

};


#endif //CHESSPP_PIECE_H
