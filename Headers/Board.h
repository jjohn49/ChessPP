//
// Created by hugh on 6/24/23.
//

#ifndef CHESSPP_BOARD_H
#define CHESSPP_BOARD_H

#include "string"
#include <unordered_map>
#include "Pieces/Piece.h"
#include "array"
#include <vector>
#include "Move.h"


using namespace std;

class Board {
    array<Piece,8> pawns;
    array<Piece,2> knights;
    unordered_map<int32_t , array<Piece*,8>>  board;
    vector<Move> moves;
public:
    Board();
    //~Board();
    Piece* getPieceAt(char x, int32_t y);
    bool isLocationValid(char x, int32_t y);
    unordered_map<int32_t , array<Piece *,8>> getBoard();
    void printBoard();
    void addMove(Move newMove);
    void printMoves();
    vector<Move> getMovesFor(Piece piece);
    vector<Move> getMovesForPawn(Piece pawn);


};


#endif //CHESSPP_BOARD_H
