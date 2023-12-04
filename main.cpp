#include <iostream>
#include "Headers/Chess.h"
#include "Headers//Board.h"

int main() {

    Chess chess{};
    Move move;
    int counter{0};

    array<char, 2> colors{'w','b'};
    while(true){
        for(char &color: colors){
            chess.colorMoveAPiece(color);
        }
    }



    chess.colorMoveAPiece('w');





    return 0;
}



