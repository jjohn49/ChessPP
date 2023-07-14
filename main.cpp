#include <iostream>
#include "Headers/Chess.h"
#include "Headers//Board.h"



int main() {

    Chess chess{};
    Move move{chess.getAllMovesForColor('w').at(0)};

    chess.tryToMovePiece(move);
    chess.board.printBoard();


    return 0;
}



