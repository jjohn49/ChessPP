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
    std::string getImagePath() override;
    static std::vector<Move> getBishopMoves(Piece::Color color, int curX,  int curY, std::shared_ptr<Piece> board[8][8]);

};


#endif //CHESSPP_BISHOP_H
