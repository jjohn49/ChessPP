//
// Created by John Johnston on 12/8/23.
//

#include "../Header/Bishop.h"

std::vector<Move> Bishop::getMoves(std::shared_ptr<Piece> (*board)[8]) {
    std::vector<Move> moves{};

    int charge[2]{-1,1};
    std::pair<int,int> curPos{this->getPosition()};

    for(int &chargeX : charge){
        for(int &chargeY : charge){
            //Move firstMove{};
            auto getConsecutive = [&](int x, int y, auto&& getConsecutive){
                if(x < 0 || x > 7 || y < 0 || y >7){
                    return;
                }

                if(this->getPieceAt(x,y,board) == nullptr){
                    moves.push_back(Move(curPos, std::make_pair(x,y)));
                    getConsecutive(x + chargeX, y + chargeY, getConsecutive);
                }else if(this->getPieceAt(x,y,board)->getColor() != this->getColor()){
                    moves.push_back(Move(curPos, std::make_pair(x,y)));
                }
            };

            getConsecutive(curPos.first + chargeX, curPos.second + chargeY, getConsecutive);

        }
    }

    return moves;
}
