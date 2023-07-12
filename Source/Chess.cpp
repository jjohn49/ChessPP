//
// Created by hugh on 7/12/23.
//

#include "../Headers/Chess.h"

Chess::Chess(){
    board = Board();
    movesForPieces = MovesForPieces(&board);
}

vector<Move> Chess::getAllMovesForColor(char color) {
    vector<Move> moves{};

    board.printBoard();

    return moves;
}
