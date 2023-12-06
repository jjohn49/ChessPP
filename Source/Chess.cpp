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
                rowStr+= "|\t\t\t|";
            }
        }
        std::cout<<rowStr<<"\n";
    }
}

void Chess::movePiece(std::pair<int, int> oldPos, std::pair<int, int> newPos) {
    std::shared_ptr<Piece> piece = this->board[oldPos.first][oldPos.second];
    board[oldPos.first][oldPos.second] = nullptr;
    board[newPos.first][newPos.second] = piece;
    piece->setNewPosition(newPos);
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
