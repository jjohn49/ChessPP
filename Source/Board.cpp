//
// Created by hugh on 6/24/23.
//

#include "../Headers/Board.h"
#include <iostream>

Board::Board(){
    //maybe make a piece called empty
    board = {
            {1,{Piece(),Piece(),Piece(),Piece(),Piece(),Piece(),Piece(),Piece()}},
            {2,{Piece("Pawn", 'w', 'a', 2),Piece(),Piece(),Piece(),Piece(),Piece(),Piece(),Piece()}},
            {3,{nullopt, Piece("Pawn", 'b', 'b', 3), nullopt, nullopt, nullopt, nullopt, nullopt}},
            {4,{nullopt, nullopt, nullopt, nullopt, nullopt, nullopt, nullopt}},
            {5,{nullopt, nullopt, nullopt, nullopt, nullopt, nullopt, nullopt}},
            {6,{nullopt, nullopt, nullopt, nullopt, nullopt, nullopt, nullopt}},
            {7,{Piece(),Piece(),Piece(),Piece(),Piece(),Piece(),Piece(),Piece()}},
            {8,{Piece(),Piece(),Piece(),Piece(),Piece(),Piece(),Piece(),Piece()}},
    };
}


void Board::printBoard() {
    for(int row = 0; row < 9; row++){
        for(int col = 'a' - 1; col< 'i'; col++){
            if(row == 0){
                cout << (char)col << "\t\t";
                continue;
            }

            if(col == 'a' - 1){
                cout << row;
                continue;
            }
            if(this->getPieceAt((char)col, row).has_value()){
                cout << this->getPieceAt(char(col), row).value().toShortString() + "\t\t";
            }else {
                cout << "Null\t\t";
            }
        }
        cout << "\n\n";
    }
}

optional<Piece> Board::getPieceAt(char x, int32_t y) {
    //cout << y << x - 'a' << "\n\n\n";

    return isLocationValid(x, y)?  this->board.at(y)[x - 'a'] : nullopt;
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

bool Board::isLocationValid(char x, int32_t y) {
    return y <= 8 && y >= 1 && x >= 'a' && x <= 'h';
}

unordered_map<int32_t, array<optional<Piece>, 8>> Board::getBoard() {
    return this->board;
}





