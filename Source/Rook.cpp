//
// Created by hugh on 12/8/23.
//

#include "../Header/Rook.h"

std::vector<Move> Rook::getMoves(std::shared_ptr<Piece> (*board)[8]) {
    std::vector<Move> moves{};

    int charge[2]{-1,1};
    std::pair<int,int> curPos{this->getPosition()};


    auto getConsecutive = [&](int x, int y, int chargeX, int chargeY, auto&& getConsecutive){
        if(x < 0 || x > 7 || y < 0 || y >7){
            return;
        }

        if(this->getPieceAt(x,y,board) == nullptr){
            moves.push_back(Move(curPos, std::make_pair(x,y)));
            getConsecutive(x + chargeX, y + chargeY, chargeX, chargeY,getConsecutive);
        }else if(this->getPieceAt(x,y,board)->getColor() != this->getColor()){
            moves.push_back(Move(curPos, std::make_pair(x,y)));
        }
    };

    for(int &c: charge){
        getConsecutive(curPos.first + c, curPos.second, c, 0 , getConsecutive);
        getConsecutive(curPos.first, curPos.second + c, 0 , c , getConsecutive);
    }

    return moves;
}
