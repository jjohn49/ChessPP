#include <iostream>
#include "Headers/Pieces/Piece.h"
#include "Headers/Board.h"
#include "Headers/Move.h"
#include "Headers/MovesForPieces.h"
#include "HEaders/MovesForPieces.h"


int main() {


    Board b = Board();
    MovesForPieces mfp{b};
    Piece pawn = b.getPieceAt('a',2).value();
    vector<Move> moves =  mfp.getMovesFor(pawn);
    for(int x = 0; x < moves.size(); x++){
        cout << moves.at(x).toString();
    }

    return 0;
}
