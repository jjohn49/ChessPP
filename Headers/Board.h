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
    vector<Piece *> capturedPieces{};
    array<Piece,16> pawns;
    array<Piece,4> knights;
    array<Piece, 2> kings;
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
    void movePiece(Move move);
    unordered_map<int32_t , array<Piece*,8>> makeNewBoardWith(Move move);


};


#endif //CHESSPP_BOARD_H
