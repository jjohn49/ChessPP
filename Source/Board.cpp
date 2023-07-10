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
             Piece("Pawn", 'w', 'h', 2),
             Piece("Pawn", 'b', 'a', 7),
             Piece("Pawn", 'b', 'b', 7),
             Piece("Pawn", 'b', 'c', 7),
             Piece("Pawn", 'b', 'd', 7),
             Piece("Pawn", 'b', 'e', 7),
             Piece("Pawn", 'b', 'f', 7),
             Piece("Pawn", 'b', 'g', 7),
             Piece("Pawn", 'b', 'h', 7)};

    knights = {
            Piece("Knight", 'w', 'b', 1),
            Piece("Knight", 'w', 'g', 1),
            Piece("Knight", 'b', 'b', 8),
            Piece("Knight", 'b', 'g', 8)
    };

    kings = {
            Piece("King", 'w', 'e', 1),
            Piece("King", 'b', 'e', 8),
    };

    bishops = {
            Piece("Bishop", 'w', 'c', 1),
            Piece("Bishop", 'w', 'f', 1),
            Piece("Bishop", 'b', 'c', 8),
            Piece("Bishop", 'b', 'f', 8),
    };
    board = {
            {1,{nullptr, &knights[0], &bishops[0], nullptr, &kings[0], &bishops[1], &knights[1], nullptr}},
            {2,{&pawns[0], &pawns[1], &pawns[2], &pawns[3], &pawns[4], &pawns[5], &pawns[6], &pawns[7]}},
            {3,{nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}},
            {4,{nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}},
            {5,{nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}},
            {6,{nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}},
            {7,{&pawns[8], &pawns[9], &pawns[10], &pawns[11], &pawns[12], &pawns[13], &pawns[14], &pawns[15]}},
            {8,{nullptr, &knights[2], &bishops[2], nullptr, &kings[1], &bishops[3], &knights[3], nullptr}},
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

void Board::movePiece(Move move){
    Piece * piece = move.getPiece();
    pair<char, int32_t> oldPosition{move.oldPosition()};
    pair<char, int32_t> newPosition{move.newPosition()};
    this->board[oldPosition.second].at(oldPosition.first - 'a') = nullptr;
    if(Piece * captured{this->board[newPosition.second].at(newPosition.first - 'a')}){
        capturedPieces.emplace_back(captured);
    }
    this->board[newPosition.second].at(newPosition.first - 'a') = piece;
}

Board Board::makeNewBoardWith(Move move){
    Board newBoard = *this;
    Piece * piece = move.getPiece();
    pair<char, int32_t> oldPosition{move.oldPosition()};
    pair<char, int32_t> newPosition{move.newPosition()};

    newBoard.board[oldPosition.second].at(oldPosition.first - 'a') = nullptr;
    newBoard.board[newPosition.second].at(newPosition.first - 'a') = piece;

    return newBoard;
}

/*Board::~Board() {

}*/





