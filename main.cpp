#include <iostream>
#include "./src/Headers/Chess.h"

int main() {
    Chess chess{true,Hard,Piece::Black};
    //Chess chess{};
    chess.play();
    return 0;
}



