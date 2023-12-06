//
// Created by John Johnston on 12/4/23.
//

#include "../Header/Pawn.h"
#include "../Header/Move.h"


std::vector<Move> Pawn::getMoves(std::shared_ptr<Piece> board[8][8]) {
    std::cout<<"hit";
    std::vector<Move> moves{};
    std::pair<int, int> currentPosition = this->getPosition();
    int row = currentPosition.first;
    int col = currentPosition.second;

    int offset = (this->color == White)? 1 : -1;

    auto getPassingMoves = [&](){
        if(board[row + offset][col] == nullptr){
            moves.push_back(Move(std::make_pair(row,col), std::make_pair(row+offset,col)));
        }
    };

    getPassingMoves();
    for(Move & move: moves){
        std::cout << move.toString();
    }
    return moves;

}
