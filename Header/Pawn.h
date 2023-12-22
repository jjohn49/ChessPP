//
// Created by John Johnston on 12/4/23.
//

#ifndef CHESSPP_PAWN_H
#define CHESSPP_PAWN_H


#include "Piece.h"
#include <optional>


class Pawn: public Piece {
public:
    Pawn(): Piece(Piece::Type::Pawn, Piece::Color::Colorless, -1, -1){};
    Pawn(Piece::Color color, int row, int col) : Piece(Piece::Type::Pawn,color,row,col){};
    std::vector<Move> getMoves(std::shared_ptr<Piece> board[8][8]) override;
    std::string getImagePath() override;
    std::optional<Move> getEnPessant(std::shared_ptr<Piece> (&board)[8][8], std::vector<Move> & allGameMoves) override;
};


#endif //CHESSPP_PAWN_H
