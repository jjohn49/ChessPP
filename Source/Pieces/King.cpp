//
// Created by hugh on 12/9/23.
//

#include "../../Header/Pieces/King.h"

std::vector<Move> King::getMoves(std::shared_ptr<Piece> (*board)[8]) {
    std::vector<Move> moves{};
    std::pair<int,int> curPos{this->getPosition()};

    std::pair<int,int> positions[8]{
        std::make_pair(curPos.first+1, curPos.second-1),
        std::make_pair(curPos.first+1, curPos.second),
        std::make_pair(curPos.first+1, curPos.second+1),
        std::make_pair(curPos.first, curPos.second-1),
        std::make_pair(curPos.first, curPos.second+1),
        std::make_pair(curPos.first-1, curPos.second-1),
        std::make_pair(curPos.first-1, curPos.second),
        std::make_pair(curPos.first-1, curPos.second+1),
    };

    for(std::pair<int,int> &pos: positions){
        if(pos.first < 0 || pos.first>7 || pos.second < 0 || pos.second > 7){
            continue;
        }
        if(getPieceAt(pos.first,pos.second, board) == nullptr || getPieceAt(pos.first, pos.second, board)->getColor() !=this->getColor()){
            moves.push_back(Move(curPos, pos));
        }
    }
    return moves;
}

std::string King::getImagePath() {
    return (getColor() == Piece::White) ?  "../assets/PNGs/With Shadow/1x/w_king_1x.png" : "../assets/PNGs/With Shadow/1x/b_king_1x.png";
}
