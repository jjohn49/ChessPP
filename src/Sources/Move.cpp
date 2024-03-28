//
// Created by hugh on 3/21/24.
//

#include "../Headers/Move.h"
#include <sstream>

Move::Move(int oldRow, int oldCol, int newRow, int newCol, std::shared_ptr<Piece> movingPiece,
           std::shared_ptr<Piece> pieceCaptured, bool isEnPessant, Move * castleMove): Move(std::make_pair(oldRow, oldCol), std::make_pair(newRow, newCol), movingPiece, pieceCaptured, isEnPessant, castleMove){}

Move::Move(pair<int, int> oldPosition, pair<int, int> newPosition, std::shared_ptr<Piece> movingPiece,
           std::shared_ptr<Piece> pieceCaptured, bool isEnPessant, Move * castleMove){
    this->oldPosition = oldPosition;
    this->newPosition = newPosition;
    this->movingPiece = movingPiece;
    this->pieceCaptured = pieceCaptured;
    this->isEnPessant = isEnPessant;
    this->castleMove = castleMove;
}

Move::Move(): Move(-1,-1,-1,-1, nullptr) {}

std::string Move::toString(){
    std::ostringstream strout;
    strout << getOldPosition().first << getOldPosition().second << " --> " << getNewPosition().first << getNewPosition().second << endl;
    return strout.str();
}

void Move::setIsEnPessant(bool value) {
    this->isEnPessant = value;
}


