//
// Created by John Johnston on 12/4/23.
//

#ifndef CHESSPP_CHESS_H
#define CHESSPP_CHESS_H

#include <iostream>
#include <vector>
#include "Piece.h"
#include "Pawn.h"
#include <memory>
#include <optional>


class Chess {
protected:
    std::vector<Piece> whitePieces{Pawn(Piece::White),
                                   Pawn(Piece::White),
                                   Pawn(Piece::White),
                                   Pawn(Piece::White),
                                   Pawn(Piece::White),
                                   Pawn(Piece::White),
                                   Pawn(Piece::White),
                                   Pawn(Piece::White)};

    std::vector<Piece> blackPieces{Pawn(Piece::Black),
                                   Pawn(Piece::Black),
                                   Pawn(Piece::Black),
                                   Pawn(Piece::Black),
                                   Pawn(Piece::Black),
                                   Pawn(Piece::Black),
                                   Pawn(Piece::Black),
                                   Pawn(Piece::Black)};

     std::shared_ptr<Piece> board[8][8]{
            {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
            {std::make_shared<Piece>(std::move(whitePieces[7])),std::make_shared<Piece>(std::move(whitePieces[6])),std::make_shared<Piece>(std::move(whitePieces[5])),std::make_shared<Piece>(std::move(whitePieces[4])),std::make_shared<Piece>(std::move(whitePieces[3])),std::make_shared<Piece>(std::move(whitePieces[2])),std::make_shared<Piece>(std::move(whitePieces[1])),std::make_shared<Piece>(std::move(whitePieces[0]))},
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

};


#endif //CHESSPP_CHESS_H
