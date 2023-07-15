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
    //~Chess();
    Chess(Board &board);
    vector<Move> getAllMovesForColor(Board board, char color);
    vector<Move> getAllMovesForColor(char color);
    bool isColorInCheck(Board &board, char color);
    bool isColorInCheck(char color);
    bool tryToMovePiece(Move move);

};


#endif //CHESSPP_CHESS_H
