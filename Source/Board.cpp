//
// Created by hugh on 6/24/23.
//

#include "../Headers/Board.h"
#include <iostream>
#include <cstring>

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

    rooks = {
            Piece("Rook", 'w', 'a', 1),
            Piece("Rook", 'w', 'g', 1),
            Piece("Rook", 'b', 'a', 8),
            Piece("Rook", 'b', 'g', 8),
    };
    queens = {
            Piece("Queen", 'w', 'd', 1),
            Piece("Queen", 'b', 'd', 8)
    };
    /*board = {
            {1,{new Piece(rooks[0]), new Piece(knights[0]), new Piece(bishops[0]), new Piece(queens[0]),new Piece(kings[0]), new Piece(bishops[1]), new Piece(knights[1]), new Piece(rooks[1])}},
            {2,{new Piece(pawns[0]), new Piece(pawns[1]), new Piece(pawns[2]), new Piece(pawns[3]), new Piece(pawns[4]), new Piece(pawns[5]), new Piece(pawns[6]), new Piece(pawns[7])}},
            {3,{nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}},
            {4,{nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}},
            {5,{nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}},
            {6,{nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}},
            {7,{new Piece(pawns[8]), new Piece(pawns[9]), new Piece(pawns[10]), new Piece(pawns[11]), new Piece(pawns[12]), new Piece(pawns[13]), new Piece(pawns[14]), new Piece(pawns[15])}},
            {8,{new Piece(rooks[2]), new Piece(knights[2]), new Piece(bishops[2]), new Piece(queens[1]), new Piece(kings[1]), new Piece(bishops[3]), new Piece(knights[3]), new Piece(rooks[3])}},
    };*/

    board = {
            {1,{nullptr, nullptr, nullptr, nullptr ,new Piece(kings[0]), nullptr, nullptr, nullptr}},
            {2,{nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}},
            {3,{nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}},
            {4,{nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}},
            {5,{nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}},
            {6,{nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}},
            {7,{nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}},
            {8,{nullptr, nullptr, nullptr, nullptr, new Piece(queens[1]), nullptr, nullptr, nullptr}},
    };
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



void Board::movePiece(Move move){
    Piece * piece = move.getPiece();
    pair<char, int32_t> oldPosition{move.oldPosition()};
    pair<char, int32_t> newPosition{move.newPosition()};

    this->board[oldPosition.second].at(oldPosition.first - 'a') = nullptr;
    if(Piece * captured{this->board[newPosition.second].at(newPosition.first - 'a')}){
        capturedPieces.emplace_back(captured);
    }



    piece->setX(newPosition.first);
    piece->setY(newPosition.second);
    this->board[newPosition.second].at(newPosition.first - 'a') = piece;

    if(move.isMoveEnPessant()){
        int offset{(move.getPiece()->color=='w')? -1:1};
        capturedPieces.emplace_back(this->getPieceAt(move.getPiece()->x, move.getPiece()->y + offset));
        board.at(newPosition.second+offset).at(newPosition.first - 'a') = nullptr;
    }

    previousMoves.emplace_back(move);
    piece->hasMoved = true;
}

void Board::revertMove(Move move) {
    Piece * piece{move.getPiece()};
    pair<char, int> oldPosition{move.oldPosition()};
    pair<char, int> newPosition{move.newPosition()};

    piece->setX(oldPosition.first);
    piece->setY(oldPosition.second);
    this->board.at(newPosition.second).at(newPosition.first) = nullptr;
    this->board.at(oldPosition.second).at(oldPosition.first) = piece;

    if(move.isMoveEnPessant() || move.didMoveCapture()){
        Piece * capturedPiece{this->capturedPieces.back()};
        this->board.at(capturedPiece->y).at(capturedPiece->x) = capturedPiece;
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
    return (color == 'w')? &kings[0] : &kings[1];
}

vector<Piece *> Board::getCapturedPieces() {
    return this->capturedPieces;
}

vector<Move> Board::getAllPreviousMoves() {
    return this->previousMoves;
}









/*Board::~Board() {

}*/





