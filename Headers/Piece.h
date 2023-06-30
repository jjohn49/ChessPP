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
    char x;
    char color;
    int32_t y;

    Piece();
    Piece(string name, char color, char x, int32_t y);

    void setX(char x);
    void setY(int32_t y);
    string toString();




};



#endif //CHESSPP_PIECE_H
