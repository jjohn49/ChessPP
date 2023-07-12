//
// Created by hugh on 7/12/23.
//

#ifndef CHESSPP_CHESS_H
#define CHESSPP_CHESS_H


#include "Pieces/Piece.h"
#include "Move.h"
#include "Board.h"
#include "MovesForPieces.h"



class Chess {
public:
    Board board;
    MovesForPieces movesForPieces;
    Chess();
    vector<Move> getAllMovesForColor(char color);

};


#endif //CHESSPP_CHESS_H
