//
// Created by hugh on 6/24/23.
//

#include "../Headers/Board.h"
#include <iostream>

Board::Board(){
    //maybe make a piece called empty
    board = {
            {1,{Piece(),Piece(),Piece(),Piece(),Piece(),Piece(),Piece(),Piece()}},
            {2,{Piece(),Piece(),Piece(),Piece(),Piece(),Piece(),Piece(),Piece()}},
            {3,{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL}},
            {4,{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL}},
            {5,{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL}},
            {6,{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL}},
            {7,{Piece(),Piece(),Piece(),Piece(),Piece(),Piece(),Piece(),Piece()}},
            {8,{Piece(),Piece(),Piece(),Piece(),Piece(),Piece(),Piece(),Piece()}}
    };
}


void Board::printBoard() {
    for(int row = 0; row < 8; row++){
        for(int col = 'a'; col< 'i'; col++){
            if(this->getPieceAt(row, col) == NULL){
                cout << "Null";
            }
        }
    }
}


Piece Board::getPieceAt(int32_t x, char y) {
    return this->board.at(x)[y];
}

