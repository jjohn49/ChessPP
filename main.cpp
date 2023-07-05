#include <iostream>
#include "Headers/Pieces/Piece.h"
#include "Headers/Board.h"
#include "Headers/Move.h"
#include "Headers/MovesForPieces.h"


int main() {


    Board b = Board();

    if (b.getPieceAt('b', 2)){
        cout << b.getPieceAt('b', 2).value().toString();
    }

    return 0;
}
