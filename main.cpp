#include <iostream>
#include "Headers/Pieces/Piece.h"
#include "Headers/Board.h"
#include "Headers/Move.h"


int main() {

    Piece piece = Piece("Pawn", 'b', 'a', 1);
    Move m = Move(piece, "a1", "b1");
    Board b = Board();
    //b.printBoard();
    b.addMove(m);
    b.printMoves();
    
    return 0;
}
