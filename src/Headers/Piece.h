//
// Created by hugh on 3/21/24.
//

#ifndef CHESSPP_PIECE_H
#define CHESSPP_PIECE_H

#include <iostream>
#include "Move.h"
#include <vector>

using namespace std;

//Forward Declaration of Board
class Board;

class Piece: public enable_shared_from_this<Piece> {
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
    void setHasMoved(bool value){this->hasMoved = value;};
    void setNewPosition(int row, int col);
    std::string toString();

    virtual vector<Move> getMoves(Board * board){return  {};};
    virtual string getImagePath(){return "";};
    virtual vector<vector<float>> getEvalBoard(){
        return{{}};
    };


};


#endif //CHESSPP_PIECE_H
