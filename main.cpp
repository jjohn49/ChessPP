#include <iostream>
#include "Headers/Pieces/Piece.h"
#include "Headers/Board.h"
#include "Headers/Move.h"
#include "Headers/MovesForPieces.h"
#include "Headers/MovesForPieces.h"


int main() {


    Board b = Board();
    MovesForPieces mfp{b};
    Piece * pawn = b.getPieceAt('a',2);
    Move m{pawn, pawn->x, pawn->y, 'a', 3};
    b.movePiece(m);
    b.printBoard();

    return 0;
}
