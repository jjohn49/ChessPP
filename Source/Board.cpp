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
            {3,{nullopt, nullopt, nullopt, nullopt, nullopt, nullopt, nullopt}},
            {4,{nullopt, nullopt, nullopt, nullopt, nullopt, nullopt, nullopt}},
            {5,{nullopt, nullopt, nullopt, nullopt, nullopt, nullopt, nullopt}},
            {6,{nullopt, nullopt, nullopt, nullopt, nullopt, nullopt, nullopt}},
            {7,{Piece(),Piece(),Piece(),Piece(),Piece(),Piece(),Piece(),Piece()}},
            {8,{Piece(),Piece(),Piece(),Piece(),Piece(),Piece(),Piece(),Piece()}},
    };
}


void Board::printBoard() {
    for(int row = 0; row < 9; row++){
        for(int col = 'a'; col< 'i'; col++){
            if(row == 0){
                cout << (char)col << "\t\t";
                continue;
            }

            if(col == 'a'){
                cout << row;
                continue;
            }
            if(this->getPieceAt(row, (char)col).has_value()){
                cout << this->getPieceAt(row, (char)col).value().toShortString() + "\t\t";
            }else {
                cout << "Null\t\t";
            }
        }
        cout << "\n\n";
    }
}


optional<Piece> Board::getPieceAt(int32_t y, char x) {
    if(y > 8 || y < 1 || x < 'a' || x > 'h'){
        return nullopt;
    }
    return this->board.at(y)[x - 'a'];
}

void Board::addMove(Move newMove) {
    moves.push_back(newMove);
}

void Board::printMoves(){
    cout << "MOVES\n";
    for(int x = 0; x < moves.size(); x++){
        cout << x + 1 << ". " << moves[x].toString() << "\n";
    }
}





