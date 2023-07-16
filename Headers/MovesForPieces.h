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
    vector<Move> getMovesFor(Piece * piece);
private:

    bool isCheck(Board board, char color);
    vector<Move> getMovesForPawn(Piece * pawn);
    vector<Move> getRegularMovesForPawn(Piece * pawn);
    vector<Move> getTakeMovesForPawn(Piece * pawn);
    void checkForEnPessant(vector<Move> * move, Piece * Pawn);

    vector<Move> getMovesForKnight(Piece * knight);

    vector<Move> getMovesForKing(Piece * king);

    void checkForCastling(Piece * king);

    vector<Move> getMovesForBishop(Piece * bishop);

    vector<Move> getMovesForRook(Piece * rook);

    vector<Move> getMovesForQueen(Piece * queen);

    void
    getConsecutiveMoves(vector<Move> *moves, Move newMove, bool vertical, bool horizontal, int chargeX, int chargeY);

    Move
    addToMovesandGetNextMove(vector<Move> *moves, Move newMove, bool horizontal, bool vertical, int chargeX,
                             int chargeY);

    vector<Move> &getVector(Piece *rook, vector<Move> &m);

    void rookLogic(Piece *rook, vector<Move> *moves);

    void bishopLogic(Piece *bishop, vector<Move> *moves);

    bool isValidMove(vector<Move> *moves, Move move);
};


#endif //CHESSPP_MOVESFORPIECES_H
