//
// Created by John Johnston on 12/4/23.
//

#ifndef CHESSPP_CHESS_H
#define CHESSPP_CHESS_H

#include <iostream>
#include "Piece.h"
#include "Pawn.h"


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
    std::vector<std::vector<Piece *>> board;
public:
    Chess();
    void play();

};


#endif //CHESSPP_CHESS_H
