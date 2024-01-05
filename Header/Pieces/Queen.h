//
// Created by hugh on 12/8/23.
//

#ifndef CHESSPP_QUEEN_H
#define CHESSPP_QUEEN_H
#include "Rook.h"
#include "Bishop.h"
#include "Piece.h"

class Queen: public Piece{
public:
    Queen(): Piece(Piece::Type::Queen, Piece::Color::Colorless, -1, -1){};
    Queen(Piece::Color color, int row, int col) : Piece(Piece::Type::Queen,color,row,col){};
    std::vector<Move> getMoves(std::shared_ptr<Piece> board[8][8]) override;
    std::string getImagePath() override;
};


#endif //CHESSPP_QUEEN_H
