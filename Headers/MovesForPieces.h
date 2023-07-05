//
// Created by John Johnston on 7/5/23.
//

#ifndef CHESSPP_MOVESFORPIECES_H
#define CHESSPP_MOVESFORPIECES_H
#include "Board.h"


class MovesForPieces {
    Board board;
    MovesForPieces(Board board);
    vector<Move> getMovesFor(Piece piece);
    vector<Move> getMovesForPawn(Piece pawn);
    vector<Move> getRegularMovesForPawn(Piece pawn);
    vector<Move> getTakeMovesForPawn(Piece pawn);
};


#endif //CHESSPP_MOVESFORPIECES_H
