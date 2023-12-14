//
// Created by John Johnston on 12/4/23.
//

#include "../Header/Pawn.h"
#include "../Header/Move.h"


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
std::vector<Move> Pawn::getEnPessant(std::shared_ptr<Piece> board[8][8], std::vector<Move> allGameMoves){
    std::vector<Move> moves{};
    int r = row;
    int c = col;

    if( (getColor() == Piece::White && r == 4) || (getColor()==Piece::Black && r == 3)){
        Move lastMove{allGameMoves.back()};
        std::pair<int,int> lastPos{lastMove.getNewPosition()};
        std::shared_ptr<Piece> lastPieceMoved{board[lastPos.first][lastPos.second]};

    }

    return moves;
}

std::string Pawn::getImagePath() {
    return (getColor() == Piece::White) ?  "../assets/PNGs/With Shadow/1x/w_pawn_1x.png" : "../assets/PNGs/With Shadow/1x/b_pawn_1x.png";
};
