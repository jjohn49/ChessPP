#include <iostream>
#include "Headers/Pieces/Piece.h"
#include "Headers/Board.h"
#include "Headers/Move.h"
#include "Headers/MovesForPieces.h"
#include "Headers/MovesForPieces.h"


int main() {


    Board b = Board();
    MovesForPieces mfp{b};
    Piece * pawn = b.getPieceAt('f',1);
    vector<Move> moves = mfp.getMovesFor(pawn);
    for(auto &move: moves){
        cout << move.toString() << endl;
    }

    return 0;
}
