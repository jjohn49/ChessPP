//
// Created by hugh on 12/8/23.
//

#include "../../Header/Pieces/Rook.h"

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

std::vector<Move> Rook::getRookMoves(Piece::Color color, int curX, int curY, std::shared_ptr<Piece> (*board)[8]) {
    Rook temp{color, curX, curY};
    return temp.getMoves(board);
}

std::string Rook::getImagePath() {
    return (getColor() == Piece::White) ?  "../assets/PNGs/With Shadow/1x/w_rook_1x.png" : "../assets/PNGs/With Shadow/1x/b_rook_1x.png";
}
