//
// Created by John Johnston on 7/5/23.
//

#ifndef CHESSPP_MOVESFORPIECES_H
#define CHESSPP_MOVESFORPIECES_H
#include "Board.h"
#include "Move.h"


class MovesForPieces {
    Board * board;
public:
    MovesForPieces();
    MovesForPieces(Board * board);
    //~MovesForPieces();

    void getMovesFor(vector<Move> &moves, Piece &piece);
private:
    void checkForEnPessant(vector<Move> & move, Piece & Pawn);

    void
    getConsecutiveMoves(vector<Move> &moves, Move newMove, bool vertical, bool horizontal, int chargeX, int chargeY);

    Move
    addToMovesandGetNextMove(vector<Move> &moves, Move newMove, bool horizontal, bool vertical, int chargeX,
                             int chargeY);


    void rookLogic(Piece &rook, vector<Move> &moves);

    void bishopLogic(Piece &bishop, vector<Move> &moves);

    bool isValidMove(vector<Move> &moves, Move move);

    void getMovesForQueen(vector<Move> &moves, Piece &queen);

    void getMovesForRook(vector<Move> &moves, Piece &rook);

    void getMovesForBishop(vector<Move> &moves, Piece &bishop);

    void getMovesForKing(vector<Move> &moves, Piece &king);

    void getMovesForKnight(vector<Move> &moves, Piece &knight);

    void getTakeMovesForPawn(vector<Move> &pawnMoves, Piece &pawn);

    void getRegularMovesForPawn(vector<Move> &pawnMoves, Piece &pawn);

    void getMovesForPawn(vector<Move> &moves, Piece &pawn);


};


#endif //CHESSPP_MOVESFORPIECES_H
