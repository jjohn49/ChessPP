//
// Created by hugh on 12/9/23.
//

#ifndef CHESSPP_KING_H
#define CHESSPP_KING_H


#include "Piece.h"

class King : public Piece{
public:
    King(): Piece(Piece::King, Piece::Colorless, -1, -1){};
    King(Piece::Color color, int row, int col): Piece(Piece::King, color, row, col){};
    std::vector<Move> getMoves(std::shared_ptr<Piece> board[8][8]) override;

};


#endif //CHESSPP_KING_H
