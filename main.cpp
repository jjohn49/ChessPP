#include <iostream>
#include "Headers/Chess.h"
#include "Headers//Board.h"



int main() {

    Chess chess{};
    Move move;
    int counter{0};

    while(chess.board.getCapturedPieces().size() == 0){
        cout << counter++ << endl;
        move=chess.getAllMovesForColor('w').at(5);
        cout << move.toString() << endl;
        chess.tryToMovePiece(move);
        chess.board.printBoard();
    }

    cout << move.toString() << endl;
    cout << chess.board.getCapturedPieces().at(0)->toString() << endl;
    chess.board.printBoard();




    return 0;
}



