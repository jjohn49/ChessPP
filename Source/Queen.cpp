//
// Created by hugh on 12/8/23.
//

#include "../Header/Queen.h"
#include "../Header/Bishop.h"
#include "../Header/Rook.h"

std::vector<Move> Queen::getMoves(std::shared_ptr<Piece> (*board)[8]) {
    std::pair<int,int> curPos{this->getPosition()};
    std::vector<Move> moves{Bishop::getBishopMoves(this->getColor(), curPos.first, curPos.second, board)};
    std::vector<Move> rookMoves{Rook::getRookMoves(this->getColor(), curPos.first, curPos.second, board)};
    moves.insert(moves.end(), rookMoves.begin(), rookMoves.end());
    return moves;
}
