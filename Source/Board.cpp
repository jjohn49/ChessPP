//
// Created by hugh on 6/24/23.
//

#include "../Headers/Board.h"
#include <iostream>

Board::Board(){
    //maybe make a piece called empty
    pawns = {Piece("Pawn", 'w', 'a', 2),
             Piece("Pawn", 'w', 'b', 2),
             Piece("Pawn", 'w', 'c', 2),
             Piece("Pawn", 'w', 'd', 2),
             Piece("Pawn", 'w', 'e', 2),
             Piece("Pawn", 'w', 'f', 2),
             Piece("Pawn", 'w', 'g', 2),
             Piece("Pawn", 'w', 'h', 2)};

    knights = {
            Piece("Knight", 'w', 'b', 1),
            Piece("Knight", 'w', 'g', 1)
    };
    board = {
            {1,{nullptr, &knights[0], nullptr, nullptr, nullptr, nullptr, &knights[1], nullptr}},
            {2,{&pawns[0], &pawns[1], &pawns[2], &pawns[3], &pawns[4], &pawns[5], &pawns[6], &pawns[7]}},
            {3,{nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}},
            {4,{nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}},
            {5,{nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}},
            {6,{nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}},
            {7,{nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}},
            {8,{nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}},
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
            cout << "\t\t";
            if(this->getPieceAt((char)col, row) != nullptr){
                cout << this->getPieceAt(char(col), row)->toShortString();
            }else {
                cout << "Null";
            }
        }
        cout << "\n\n";
    }
}

Piece* Board::getPieceAt(char x, int32_t y) {
    //cout << y << x - 'a' << "\n\n\n";

    return isLocationValid(x, y) ?  this->board.at(y)[x - 'a'] : nullptr;
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

unordered_map<int32_t, array<Piece*, 8>> Board::getBoard() {
    return this->board;
}

/*Board::~Board() {

}*/





