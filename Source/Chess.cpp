//
// Created by John Johnston on 12/4/23.
//

#include "../Header/Chess.h"
#include "../Header/Move.h"

Chess::Chess() {

}

void Chess::printBoard() {
    for( auto & row: this->board){
        std::string rowStr = "";
        for(std::shared_ptr<Piece> & square: row){
            if(square != nullptr){
                rowStr += square->toString() + "\t";
            }else{
                rowStr+= "|\t\t\t\t|";
            }
        }
        std::cout<<rowStr<<"\n";
    }
}

void Chess::movePiece(Move move) {
    std::pair<int,int> oldPos{move.getOldPosition()};
    std::pair<int,int> newPos{move.getNewPosition()};
    std::shared_ptr<Piece> piece = getPieceAt(oldPos);

    //checvks if there was a piece on the square and adds it to the correct capturePieces vector
    if(this->getPieceAt(move.getNewPosition()) != nullptr){
        if(this->getPieceAt(move.getNewPosition())->getColor() == Piece::Black){
            this->captureBlackPieces.push_back(this->getPieceAt(move.getNewPosition()));
        }else{
            this->captureWhitePieces.push_back(this->getPieceAt(move.getNewPosition()));
        }
    }
    this->board[oldPos.first][oldPos.second] = nullptr;
    this->board[newPos.first][newPos.second] = piece;
    piece->setNewPosition(newPos);

    this->allGameMoves.push_back(move);
}



std::shared_ptr<Piece> Chess::getPieceAt(std::pair<int, int> pos) {
    return this->board[pos.first][pos.second];
}

std::vector<Move> Chess::getAllMoves() {
    std::vector<Move> moves{};

    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            if(this->getPieceAt(std::make_pair(i,j)) != nullptr){
                std::shared_ptr<Piece> piece = this->getPieceAt(std::make_pair(i,j));
                std::vector<Move> pieceMoves = piece->getMoves(this->board);
                moves.insert(moves.end(), pieceMoves.begin(), pieceMoves.end());
            }

        }
    }

    return moves;
}

void Chess::printAllMoves() {
    for(Move & move: this->getAllMoves()){
        std::cout << move.toString();
    }
}

bool Chess::isCheckFor(Piece::Color color) {
    std::vector<Move> allMoves{this->getAllMoves()};

    std::shared_ptr<King> targetKing = (color == Piece::White) ? std::make_shared<King>(whiteKing) : std::make_shared<King>(blackKing);

    for(Move &move: allMoves){
        if(move.getNewPosition() == targetKing->getPosition()){
            //free(targetKing);
            return true;
        }
    }

    //free(targetKing);
    return false;
}

//Gets all possible moves, makes a copy of the board before any move
//Then modifies board with each move, checking if they are still in check
//resets to original board after each move
std::vector<Move> Chess::getMovesFor(Piece::Color color) {
    std::vector<Move> allMoves{getAllMoves()};
    std::vector<Move> movesForColor{};
    for(Move & move: allMoves){
        if(getPieceAt(move.getOldPosition())->getColor() == color){
            std::shared_ptr<Piece> copy[8][8];
            std::vector<Move> copyAllGameMoves = this->allGameMoves;
            std::copy(&board[0][0], &board[0][0]+64, &copy[0][0]);
            movePiece(move);
            if(!isCheckFor(color)){
                movesForColor.push_back(move);
            }
            std::copy(&copy[0][0], &copy[0][0]+64, &board[0][0]);
            this->allGameMoves = copyAllGameMoves;
        }
    }

    return movesForColor;
}

