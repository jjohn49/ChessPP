//
// Created by John Johnston on 12/4/23.
//

#ifndef CHESSPP_CHESS_H
#define CHESSPP_CHESS_H

#include <iostream>
#include <vector>
#include "Pawn.h"
#include <memory>
#include <optional>


class Chess {
protected:
    std::vector<Pawn> whitePieces{Pawn(Piece::White,1,0),
                                   Pawn(Piece::White,1,1),
                                   Pawn(Piece::White,1,2),
                                   Pawn(Piece::White,1,3),
                                   Pawn(Piece::White,1,4),
                                   Pawn(Piece::White,1,5),
                                   Pawn(Piece::White,1,6),
                                   Pawn(Piece::White,1,7)};

    std::vector<Piece> blackPieces{Pawn(Piece::Black,6,0),
                                   Pawn(Piece::Black,6,1),
                                   Pawn(Piece::Black,6,2),
                                   Pawn(Piece::Black,6,3),
                                   Pawn(Piece::Black,6,4),
                                   Pawn(Piece::Black,6,5),
                                   Pawn(Piece::Black,6,6),
                                   Pawn(Piece::Black,6,7)};

     std::shared_ptr<Piece> board[8][8]{
            {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
            {std::make_shared<Pawn>(whitePieces[0]),std::make_shared<Pawn>(whitePieces[1]),std::make_shared<Pawn>(whitePieces[2]),std::make_shared<Pawn>(whitePieces[3]),std::make_shared<Pawn>(whitePieces[4]),std::make_shared<Pawn>(whitePieces[5]),std::make_shared<Pawn>(whitePieces[6]),std::make_shared<Pawn>(whitePieces[7])},
            //{nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
            {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
            {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
            {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
            {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
            {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
            {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}
    };
public:
    Chess();
    void play();
    void printBoard();
    std::shared_ptr<Piece> getPieceAt(std::pair<int,int> pos);
    void movePiece(std::pair<int,int> oldPos, std::pair<int,int> newPos);
    std::vector<Move> getAllMoves();
    void printAllMoves();

};


#endif //CHESSPP_CHESS_H
