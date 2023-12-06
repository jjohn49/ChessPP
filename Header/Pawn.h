//
// Created by John Johnston on 12/4/23.
//

#ifndef CHESSPP_PAWN_H
#define CHESSPP_PAWN_H


#include "Piece.h"


class Pawn: public Piece {
public:
    Pawn(): Piece(Piece::Type::Pawn, Piece::Color::Colorless, -1, -1){};
    Pawn(Piece::Color color, int row, int col) : Piece(Piece::Type::Pawn,color,row,col){};
    std::vector<Move> getMoves(std::shared_ptr<Piece> board[8][8]) override;
};


#endif //CHESSPP_PAWN_H
