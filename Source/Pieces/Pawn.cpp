//
// Created by John Johnston on 12/4/23.
//

#include "../../Header/Pieces/Pawn.h"
#include "../../Header/Move.h"


std::vector<Move> Pawn::getMoves(std::shared_ptr<Piece> board[8][8]) {
    //std::cout<<"hit";
    std::vector<Move> moves{};
    std::pair<int, int> currentPosition = this->getPosition();
    int row = currentPosition.first;
    int col = currentPosition.second;

    int offset = (this->color == White)? 1 : -1;

    //lambda func to get all passing moves
    auto getPassingMoves = [&](){
        if(board[row + offset][col] == nullptr){
            moves.push_back(Move(std::make_pair(row,col), std::make_pair(row+offset,col)));
            if(!this->hasMoved && board[row + (2*offset)][col] == nullptr){
                moves.push_back(Move(std::make_pair(row,col), std::make_pair(row+(2*offset),col)));
            }
        }
    };

    auto getTakeMoves = [&](){
        int lrOffset[2] = {-1,1};
        for(int & lr: lrOffset){
            if(board[row + offset][col + lr] != nullptr && board[row + offset][col + lr]->getColor() != this->color){
                moves.push_back(Move(std::make_pair(row,col),std::make_pair(row + offset, col + lr)));
            }
        }
    };


    getPassingMoves();
    getTakeMoves();

    return moves;

}

//NOT FINISHED YET
std::optional<Move> Pawn::getEnPessant(std::shared_ptr<Piece> (&board)[8][8], std::vector<Move> & allGameMoves){
    if(allGameMoves.empty() || board[allGameMoves.back().getNewPosition().first][allGameMoves.back().getNewPosition().second]->getType() != Piece::Pawn){
        return std::nullopt;
    }

    Move lastMove{allGameMoves.back()};
    int r = row;
    int c = col;

    //Know it's a black pawn because white pawns cannot go from row 6 -> row 4
    if(getColor() == Piece::White && r == 4){
        if(lastMove.getOldPosition().first == 6 && lastMove.getNewPosition().first == 4){
            return Move(this->getPosition(),std::make_pair(5,lastMove.getNewPosition().second), true);
        }
    }
    else if(getColor() == Piece::Black && r == 3){
        if(lastMove.getOldPosition().first == 1 && lastMove.getNewPosition().first == 3){
            return Move(this->getPosition(),std::make_pair(2,lastMove.getNewPosition().second), true);
        }
    }

    return std::nullopt;
}

std::string Pawn::getImagePath() {
    return (getColor() == Piece::White) ?  "../assets/PNGs/With Shadow/1x/w_pawn_1x.png" : "../assets/PNGs/With Shadow/1x/b_pawn_1x.png";
};
