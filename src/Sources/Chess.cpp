//
// Created by hugh on 3/21/24.
//

#include "../Headers/Chess.h"

Chess::Chess() {
    board = Board();
}

void Chess::play() {
    board.print();
}
