//
// Created by hugh on 3/21/24.
//

#include "Board.h"

Board::Board() {
    board = {
            {make_shared<Piece>(),make_shared<Piece>(),make_shared<Piece>(),make_shared<Piece>(),make_shared<Piece>(),make_shared<Piece>(),make_shared<Piece>(),make_shared<Piece>()},
            {make_shared<Piece>(),make_shared<Piece>(),make_shared<Piece>(),make_shared<Piece>(),make_shared<Piece>(),make_shared<Piece>(),make_shared<Piece>(),make_shared<Piece>()},
            {make_shared<Piece>(),make_shared<Piece>(),make_shared<Piece>(),make_shared<Piece>(),make_shared<Piece>(),make_shared<Piece>(),make_shared<Piece>(),make_shared<Piece>()},
            {make_shared<Piece>(),make_shared<Piece>(),make_shared<Piece>(),make_shared<Piece>(),make_shared<Piece>(),make_shared<Piece>(),make_shared<Piece>(),make_shared<Piece>()},
            {make_shared<Piece>(),make_shared<Piece>(),make_shared<Piece>(),make_shared<Piece>(),make_shared<Piece>(),make_shared<Piece>(),make_shared<Piece>(),make_shared<Piece>()},
            {make_shared<Piece>(),make_shared<Piece>(),make_shared<Piece>(),make_shared<Piece>(),make_shared<Piece>(),make_shared<Piece>(),make_shared<Piece>(),make_shared<Piece>()},
            {make_shared<Piece>(),make_shared<Piece>(),make_shared<Piece>(),make_shared<Piece>(),make_shared<Piece>(),make_shared<Piece>(),make_shared<Piece>(),make_shared<Piece>()},
            {make_shared<Piece>(),make_shared<Piece>(),make_shared<Piece>(),make_shared<Piece>(),make_shared<Piece>(),make_shared<Piece>(),make_shared<Piece>(),make_shared<Piece>()},
    };
}

Piece *Board::getPieceAt(int row, int col) {
    return board[row][col].get();
}

Piece *Board::getPieceAt(pair<int, int> position){
    return getPieceAt(position.first, position.second);
}


