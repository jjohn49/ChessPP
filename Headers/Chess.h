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
    Board * board;
    MovesForPieces * movesForPieces;
    Chess();
    ~Chess();
    void play();
    Chess(Board &board);
    vector<Move> getAllMovesForColor(Board board, char color);
    vector<Move> getAllMovesForColor(char color);
    bool isColorInCheck(Board * board, char color);
    bool isColorInCheck(char color);
    void movePiece(Move move);
    void colorMoveAPiece(char color);
    void checkForEnPassant(vector<Move> & moves);

    bool isCheckMate(char color);

    vector<Move> getAllLegalMovesFor(char color);

    void checkForCastling(vector<Move> &moves);

    void checkForCastling(vector<Move> &moves, char color);

    void castlingLogic(vector<Move> &moves, Piece &king, Piece &rook);

    bool colorTryToMovePiece(char color);
};


#endif //CHESSPP_CHESS_H
