//
// Created by John Johnston on 12/8/23.
//

#ifndef CHESSPP_BISHOP_H
#define CHESSPP_BISHOP_H

#include "Piece.h"

class Bishop: public Piece{
public:
    Bishop(): Piece(Piece::Type::Bishop, Piece::Color::Colorless, -1, -1){};
    Bishop(Piece::Color color, int row, int col) : Piece(Piece::Type::Bishop,color,row,col){};
    std::vector<Move> getMoves(std::shared_ptr<Piece> board[8][8]) override;

};


#endif //CHESSPP_BISHOP_H
