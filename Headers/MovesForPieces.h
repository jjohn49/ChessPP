//
// Created by John Johnston on 7/5/23.
//

#ifndef CHESSPP_MOVESFORPIECES_H
#define CHESSPP_MOVESFORPIECES_H
#include "Board.h"


class MovesForPieces {
    Board board;
public:
    MovesForPieces(Board board);
    vector<Move> getMovesFor(Piece * piece);
private:
    vector<Move> getMovesForPawn(Piece * pawn);
    vector<Move> getRegularMovesForPawn(Piece * pawn);
    vector<Move> getTakeMovesForPawn(Piece * pawn);

    vector<Move> getMovesForKnight(Piece * Knight);
};


#endif //CHESSPP_MOVESFORPIECES_H
