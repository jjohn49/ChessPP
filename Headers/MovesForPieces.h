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
    bool isValidMove(Piece * piece, char x, int32_t y);
    bool isCheck(unordered_map<int32_t , array<Piece*,8>>  newBoard);
    vector<Move> getMovesForPawn(Piece * pawn);
    vector<Move> getRegularMovesForPawn(Piece * pawn);
    vector<Move> getTakeMovesForPawn(Piece * pawn);

    vector<Move> getMovesForKnight(Piece * knight);

    vector<Move> getMovesForKing(Piece * king);
};


#endif //CHESSPP_MOVESFORPIECES_H
