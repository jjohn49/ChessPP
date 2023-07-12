#include <iostream>
#include "Headers/Chess.h"




int main() {

    Chess chess{};
    vector<Move> moves{chess.getAllMovesForColor('w')};
    cout << moves.size();



    return 0;
}
