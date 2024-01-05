//
// Created by hugh on 12/8/23.
//

#include "../../Header/Pieces/Queen.h"
#include "../../Header/Pieces/Bishop.h"
#include "../../Header/Pieces/Rook.h"

std::vector<Move> Queen::getMoves(std::shared_ptr<Piece> (*board)[8]) {
    std::pair<int,int> curPos{this->getPosition()};
    std::vector<Move> moves{Bishop::getBishopMoves(this->getColor(), curPos.first, curPos.second, board)};
    std::vector<Move> rookMoves{Rook::getRookMoves(this->getColor(), curPos.first, curPos.second, board)};
    moves.insert(moves.end(), rookMoves.begin(), rookMoves.end());
    return moves;
}

std::string Queen::getImagePath() {
    return (getColor() == Piece::White) ?  "../assets/PNGs/With Shadow/1x/w_queen_1x.png" : "../assets/PNGs/With Shadow/1x/b_queen_1x.png";
}
