//
// Created by hugh on 6/24/23.
//

#include "../Headers/Board.h"

#include <iostream>
#include <cstring>

Board::Board(){
    //maybe make a piece called empty
    pawns = {new Piece("Pawn", 'w', 'a', 2),
             new Piece("Pawn", 'w', 'b', 2),
             new Piece("Pawn", 'w', 'c', 2),
             new Piece("Pawn", 'w', 'd', 2),
             new Piece("Pawn", 'w', 'e', 2),
             new Piece("Pawn", 'w', 'f', 2),
             new Piece("Pawn", 'w', 'g', 2),
             new Piece("Pawn", 'w', 'h', 2),
             new Piece("Pawn", 'b', 'a', 7),
             new Piece("Pawn", 'b', 'b', 7),
             new Piece("Pawn", 'b', 'c', 7),
             new Piece("Pawn", 'b', 'd', 7),
             new Piece("Pawn", 'b', 'e', 7),
             new Piece("Pawn", 'b', 'f', 7),
             new Piece("Pawn", 'b', 'g', 7),
             new Piece("Pawn", 'b', 'h', 7)};

    knights = {
            new Piece("Knight", 'w', 'b', 1),
            new Piece("Knight", 'w', 'g', 1),
            new Piece("Knight", 'b', 'b', 8),
            new Piece("Knight", 'b', 'g', 8)
    };

    kings = {
            new Piece("King", 'w', 'e', 1),
            new Piece("King", 'b', 'e', 8),
    };

    bishops = {
            new Piece("Bishop", 'w', 'c', 1),
            new Piece("Bishop", 'w', 'f', 1),
            new Piece("Bishop", 'b', 'c', 8),
            new Piece("Bishop", 'b', 'f', 8),
    };

    rooks = {
            new Piece("Rook", 'w', 'a', 1),
            new Piece("Rook", 'w', 'h', 1),
            new Piece("Rook", 'b', 'a', 8),
            new Piece("Rook", 'b', 'h', 8),
    };
    queens = {
            new Piece("Queen", 'w', 'd', 1),
            //Piece("Queen", 'b', 'd', 8)
            new Piece("Queen", 'b', 'e', 8)
    };
    board = {
            {1,{rooks[0], knights[0], bishops[0], queens[0],kings[0], bishops[1], knights[1], rooks[1]}},
            {2,{pawns[0], pawns[1], pawns[2], pawns[3], pawns[4], pawns[5], pawns[6], pawns[7]}},
            {3,{nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}},
            {4,{nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}},
            {5,{nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}},
            {6,{nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}},
            {7,{pawns[8], pawns[9], pawns[10], pawns[11], pawns[12], pawns[13], pawns[14], pawns[15]}},
            {8,{rooks[2], knights[2], bishops[2], queens[1], kings[1], bishops[3], knights[3], rooks[3]}},
    };

    //TEST BOARD
    /*
    board = {
            {1,{rooks[0], nullptr, nullptr, nullptr ,kings[0], nullptr, nullptr, rooks[1]}},
            {2,{nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}},
            {3,{nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}},
            {4,{nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}},
            {5,{nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}},
            {6,{nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}},
            {7,{nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}},
            {8,{rooks[2], nullptr, nullptr, nullptr, kings[1], nullptr, nullptr, rooks[3]}},
    };*/
    previousMoves = vector<Move>();
}



void Board::printBoard() {
    for(int row = 0; row < 9; row++){
        for(int col = 'a' - 1; col< 'i'; col++){
            if(row == 0){
                cout << (char)col << "\t\t\t";
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

    return isLocationValid(x, y) ?  this->board.at(y).at(x - 'a') : nullptr;
}

void Board::addMove(Move newMove) {
    previousMoves.push_back(newMove);
}

void Board::printMoves(){
    cout << "MOVES\n";
    for(int x = 0; x < currentMoves.size(); x++){
        cout << x + 1 << ". " << previousMoves[x].toString() << "\n";
    }
}

bool Board::isLocationValid(char x, int32_t y) {
    return (y <= 8 && y >= 1 && x >= 'a' && x <= 'h');
}

unordered_map<int32_t, array<Piece*, 8>> Board::getBoard() {
    return this->board;
}

void Board::movePiece(Move move, bool isTest){
    Piece * piece = move.getPiece();
    pair<char, int32_t> oldPosition{move.oldPosition()};
    pair<char, int32_t> newPosition{move.newPosition()};

    this->board[oldPosition.second].at(oldPosition.first - 'a') = nullptr;
    if(Piece * captured{this->board.at(newPosition.second).at(newPosition.first - 'a')}){
        capturedPieces.emplace_back(captured);
    }

    piece->x = newPosition.first;
    piece->y = newPosition.second;
    this->board[newPosition.second].at(newPosition.first - 'a') = piece;

    if(move.isMoveEnPessant()){
        int offset{(move.getPiece()->color=='w')? -1:1};
        capturedPieces.emplace_back(this->getPieceAt(move.getPiece()->x, move.getPiece()->y + offset));
        board.at(newPosition.second+offset).at(newPosition.first - 'a') = nullptr;
    }

    previousMoves.emplace_back(move);
    if(!isTest){
        piece->hasMoved = true;
    }
}

void Board::movePiece(Move move){
    this->movePiece(move, false);

}

void Board::revertMove(Move move) {
    Piece * piece{move.getPiece()};
    pair<char, int> oldPosition{move.oldPosition()};
    pair<char, int> newPosition{move.newPosition()};

    piece->setX(oldPosition.first);
    piece->setY(oldPosition.second);
    this->board.at(newPosition.second).at(newPosition.first - 'a') = nullptr;
    this->board.at(oldPosition.second).at(oldPosition.first - 'a') = piece;

    if(move.isMoveEnPessant() || move.didMoveCapture()){
        Piece * capturedPiece{this->capturedPieces.back()};
        this->board.at(capturedPiece->y).at(capturedPiece->x - 'a') = capturedPiece;
        capturedPieces.erase(capturedPieces.end());
    }
}

/*Board Board::makeNewBoardWith(Move move){
    Board& newBoard = *this;

    Piece * piece = move.getPiece();
    pair<char, int32_t> oldPosition{move.oldPosition()};
    pair<char, int32_t> newPosition{move.newPosition()};
    Piece * newPiece = newBoard.getPieceAt(oldPosition.first, oldPosition.second);
    newPiece->setX(newPosition.first);
    newPiece->setY(newPosition.second);
    newBoard.board.at(oldPosition.second).at(oldPosition.first - 'a') = nullptr;
    newBoard.board.at(newPosition.second).at(newPosition.first - 'a') = newPiece;

    return newBoard;
}*/

Piece * Board::getKingForColor(char color) {
    return (color == 'w')? kings[0] : kings[1];
}

vector<Piece *> Board::getCapturedPieces() {
    return this->capturedPieces;
}

vector<Move> Board::getAllPreviousMoves() {
    return this->previousMoves;
}









/*Board::~Board() {

}*/






