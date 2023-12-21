//
// Created by John Johnston on 12/4/23.
//

#include "../Header/Chess.h"
#include "../Header/Move.h"

Chess::Chess() {
    //board.printBoard();
}

void Chess::printBoard() {
    board.printBoard();
}

void Chess::movePiece(Move move) {
    board.movePiece(move);
}

void Chess::play() {

}

void Chess::drawBoard() {
    this->board.onExecute();
}




