#include <iostream>
#include "./src/Headers/Chess.h"

int main() {
    Chess chess{true,Easy,Piece::Black};
    chess.play();
    return 0;
}



