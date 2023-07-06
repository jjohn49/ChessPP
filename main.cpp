#include <iostream>
#include "Headers/Pieces/Piece.h"
#include "Headers/Board.h"
#include "Headers/Move.h"
#include "Headers/MovesForPieces.h"
#include "Headers/MovesForPieces.h"


int main() {


    Board b = Board();
    MovesForPieces mfp{b};
    Piece pawn = b.getPieceAt('a',2).value();
    //cout << pawn.toString();
    vector<Move> moves =  mfp.getMovesFor(pawn);
    cout << "MOVE \n";
    for(int x = 0; x < moves.size(); x++){
        cout << moves.at(x).toString() << endl;
    }

    return 0;
}
