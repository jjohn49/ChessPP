//
// Created by John Johnston on 3/29/24.
//

#include "../Headers/Player.h"

Player::Player() {
    this->color = Piece::NoColor;
    this->board = nullptr;
    this->piecesCaptured = {};
    this->timeLeft = 600.00;
}


Player::Player(Piece::Color color, Board * board) {
    this->color = color;
    this->board = board;
    this->piecesCaptured = {};
    this->timeLeft = 600.00;
}

void Player::addPieceCaptured(shared_ptr<Piece> piece) {
    piecesCaptured.push_back(piece);
}

vector<shared_ptr<Piece>> Player::getPiecesCaptured() {
    return this->piecesCaptured;
}

Piece::Color Player::getColor() {
    return this->color;
}

bool Player::movePiece(Move &move) {
    if(canPieceMoveThere(move)){
        board->movePiece(move);
        if(move.getCapturedPiece() != nullptr){
            addPieceCaptured(move.getCapturedPiece());
        }
        return true;
    }else{
        board->setPieceAt(move.getOldPosition(), move.getMovingPiece());
        return false;
    }
}

bool Player::canPieceMoveThere(Move &attemptedMove) {
    for(Move & m: attemptedMove.getMovingPiece()->getMoves(board)){
        //TODO: Add Method to Check if Move Creates Check

        if(m.getNewPosition() == attemptedMove.getNewPosition() && !isInCheck(attemptedMove.getMovingPiece(), attemptedMove.getNewPosition())){
            attemptedMove = m;
            return true;
        }
    }
    return false;
}

bool Player::isInCheck(shared_ptr<Piece> pieceDragging, pair<int, int> position) {
    bool ret;
    Board copyBoard = *board;
    copyBoard.setPieceAt(position, pieceDragging);
    pair<int,int> oldPos = pieceDragging->getPosition();
    pieceDragging->setNewPosition(position.first, position.second);
    ret = copyBoard.isColorInCheck(getColor());
    pieceDragging->setNewPosition(oldPos.first,oldPos.second);
    return ret;
}

bool Player::isCheckMated() {
    for(Move & m: board->getAllMovesForColor(getColor())){
        if(!isInCheck(m.getMovingPiece(), m.getNewPosition())){
            return false;
        }
    }

    return true;
}

vector<Move> Player::getAllMoves() {
    vector<Move> ret = {};
    for(Move & move: board->getAllMovesForColor(getColor())){
        if(!isInCheck(move.getMovingPiece(),move.getNewPosition())){
            ret.push_back(move);
        }
    }
    return ret;
}

bool Player::isBot() {
    return false;
}

bool Player::move() {
    //std::cout<<"nothing";
    return false;
}

Board * Player::getBoard() {
    return board;
}

int Player::getTime() {
    return timeLeft;
}

void Player::setTime(float newTime) {
    this->timeLeft = newTime;
}