//
// Created by hugh on 3/21/24.
//

#include "../Headers/Move.h"
#include <sstream>
#include "../Headers/Piece.h"

Move::Move(int oldRow, int oldCol, int newRow, int newCol, std::shared_ptr<Piece> movingPiece,
           std::shared_ptr<Piece> pieceCaptured, bool isEnPessant, bool isQueenSideCastle, bool isKingSideCastle, bool isPawnPromotion): Move(std::make_pair(oldRow, oldCol), std::make_pair(newRow, newCol), movingPiece, pieceCaptured, isEnPessant, isQueenSideCastle, isKingSideCastle, isPawnPromotion){}

Move::Move(pair<int, int> oldPosition, pair<int, int> newPosition, std::shared_ptr<Piece> movingPiece,
           std::shared_ptr<Piece> pieceCaptured, bool isEnPessant, bool isQueenSideCastle, bool isKingSideCastle, bool isPawnPromotion){
    this->oldPosition = oldPosition;
    this->newPosition = newPosition;
    this->movingPiece = movingPiece;
    this->pieceCaptured = pieceCaptured;
    this->isEnPessant = isEnPessant;
    this->isQueenSideCastle = isQueenSideCastle;
    this->isKingSideCastle = isKingSideCastle;
    this->isPawnPromotion = isPawnPromotion;
    this->promotedToPiece = nullptr;
}

Move::Move(): Move(-1,-1,-1,-1, nullptr) {}

std::string Move::toString(){
    string ret = "";

    if(this->getMovingPiece()->getType() == Piece::Knight){
        ret+= 'N';
    }else if(this->getMovingPiece()->getType() != Piece::Pawn){
        ret += this->getMovingPiece()->toString()[0];
    }

    ret+= static_cast<char>('A' + this->getOldPosition().second);
    ret+= to_string(this->getOldPosition().first + 1);

    if(this->getCapturedPiece() != nullptr){
        ret += 'x';
    }

    ret+= static_cast<char>('a' + this->getNewPosition().second);
    ret+= to_string(this->getNewPosition().first + 1);

    //to lazy to find out what the pawn gets promoted too;
    if(isPawnPromotion){
        ret+="=";
        if(promotedToPiece->getType()==Piece::Knight){
            ret+="N";
        } else if(promotedToPiece->getType() == Piece::Queen){
            ret+="Q";
        }else{
            ret+="R";
        }
    }

    return ret;
}


void Move::setIsEnPessant(bool value) {
    this->isEnPessant = value;
}

bool Move::getIsQueenSideCastle() {
    return this->isQueenSideCastle;
}

bool Move::getIsKingSideCastle() {
    return this->isKingSideCastle;
}

bool Move::getIsPawnPromotion() {return this->isPawnPromotion;}

void Move::setIsPawnPromotion(bool value) {isPawnPromotion = value;}

void Move::setPromotedToPiece(shared_ptr<Piece> piece) {
    this->promotedToPiece = piece;
}




