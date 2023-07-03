#include <iostream>
#include "Headers/Board.h"

int main() {

    Piece A = Piece("Pawn", 'b', 'a', 1);
    Piece B = Piece("Pawn", 'b', 'a', 1);

    optional<Piece> C = nullopt;

    std::cout << (A == C) << endl;
    return 0;
}
