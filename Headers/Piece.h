//
// Created by hugh on 6/24/23.
//

#ifndef CHESSPP_PIECE_H
#define CHESSPP_PIECE_H

#include <string>

using namespace std;

class Piece {
public:
    string name;
    int32_t x;
    char color;
    char y;

    Piece();
    Piece(string name, int32_t x, char y);

    void setX(char x);
    void setY(int32_t y);
    string toString();




};



#endif //CHESSPP_PIECE_H
