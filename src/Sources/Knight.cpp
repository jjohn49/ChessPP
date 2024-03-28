//
// Created by John Johnston on 3/28/24.
//

#include "../Headers/Knight.h"

Knight::Knight():Piece(){}
Knight::Knight(int row, int col, Color color): Piece(row,col,color,Type::Knight){}
Knight::Knight(pair<int,int> position, Color color): Knight(position.first,position.second,color){}

vector<Move> Knight::getMoves(Board * board) {
    return {};
}
string Knight::getImagePath() {
    return (getColor()==White)? "../assets/PNGs/No shadow/2x/w_knight_2x_ns.png":"../assets/PNGs/No shadow/2x/b_knight_2x_ns.png";
}