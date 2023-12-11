//
// Created by hugh on 12/9/23.
//

#ifndef CHESSPP_KNIGHT_H
#define CHESSPP_KNIGHT_H


#include "Piece.h"

class Knight: public Piece{
public:
    Knight(): Piece(Piece::Knight, Piece::Colorless, -1,-1){};
    Knight(Piece::Color color, int row, int col): Piece(Piece::Knight, color, row, col){};
    std::vector<Move> getMoves(std::shared_ptr<Piece> board[8][8]) override;
};


#endif //CHESSPP_KNIGHT_H
