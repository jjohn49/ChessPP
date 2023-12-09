//
// Created by hugh on 12/8/23.
//

#ifndef CHESSPP_ROOK_H
#define CHESSPP_ROOK_H
#include "Piece.h"

class Rook : public Piece{
public:
    Rook(): Piece(Piece::Type::Rook, Piece::Color::Colorless, -1, -1){};
    Rook(Piece::Color color, int row, int col) : Piece(Piece::Type::Rook,color,row,col){};
    std::vector<Move> getMoves(std::shared_ptr<Piece> board[8][8]) override;
    static std::vector<Move> getRookMoves(Piece::Color color, int curX,  int curY, std::shared_ptr<Piece> board[8][8]);

};


#endif //CHESSPP_ROOK_H
