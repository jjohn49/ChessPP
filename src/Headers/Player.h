//
// Created by John Johnston on 3/29/24.
//

#ifndef CHESSPP_PLAYER_H
#define CHESSPP_PLAYER_H

#include <iostream>
#include "Piece.h"
#include "Board.h"


class Player {
    Piece::Color color;
    Board * board;
    vector<shared_ptr<Piece>> piecesCaptured;
    float timeLeft;
public:
    Player();
    Player(Piece::Color color, Board * board);
    vector<shared_ptr<Piece>> getPiecesCaptured();
    void addPieceCaptured(shared_ptr<Piece> piece);
    Piece::Color getColor();
    vector<Move> getAllMoves();

    virtual bool movePiece(Move & move);
    bool canPieceMoveThere(Move & attemptedMove);
    bool isInCheck(shared_ptr<Piece> pieceDragging, pair<int,int> position);
    bool isCheckMated();
    Board * getBoard();
    virtual bool isBot();

    int getTime();
    void setTime(float newTime);

    //for the bot sub class
    virtual void move();
};


#endif //CHESSPP_PLAYER_H
