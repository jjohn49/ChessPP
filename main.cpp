#include <iostream>
#include "Header/Chess.h"
#include "Header/Board.h"

int main() {
    Chess chess{};
    chess.printBoard();
    std::vector<Move> moves = chess.getMovesFor(Piece::White);
    return 0;
}



