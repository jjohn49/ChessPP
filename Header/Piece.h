//
// Created by John Johnston on 12/4/23.
//

#ifndef CHESSPP_PIECE_H
#define CHESSPP_PIECE_H
#include <iostream>
#include <vector>
#include <unordered_map>
#include <memory>

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
        return this->colorToString.at(this->color) + " " + this->typeToString.at(this->type);
    };

    std::vector<Move> getMoves(std::shared_ptr<Piece> board[8][8]);

protected:
    Type type;
    Color color;
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
