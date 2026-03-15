//
// Created by John Johnston on 3/29/24.
//

#include "../Headers/Player.h"

Player::Player() {
    this->color = Piece::NoColor;
    this->board = nullptr;
    this->piecesCaptured = {};
    this->timeLeft = -1.0f;
}


Player::Player(Piece::Color color, Board * board) {
    this->color = color;
    this->board = board;
    this->piecesCaptured = {};
    this->timeLeft = -1.0f;
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
        cout << "Board eval is: " << to_string(board->evaluate()) << endl;
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
    Board copyBoard = board->deepCopy();

    // The piece may have been removed from the board grid (e.g. during dragging).
    // Look it up on the copy; if not found, temporarily place the original on the copy
    // so we can simulate the move.
    shared_ptr<Piece> copyPiece = copyBoard.getPieceAt(pieceDragging->getPosition());
    if(copyPiece == nullptr || copyPiece->getType() != pieceDragging->getType()
       || copyPiece->getColor() != pieceDragging->getColor()) {
        // Piece not on the copy board — use the original directly on the copy
        copyBoard.setPieceAt(position, pieceDragging);
        pair<int,int> oldPos = pieceDragging->getPosition();
        pieceDragging->setNewPosition(position.first, position.second);
        bool ret = copyBoard.isColorInCheck(getColor());
        pieceDragging->setNewPosition(oldPos.first, oldPos.second);
        return ret;
    }

    copyBoard.setPieceAt(position, copyPiece);
    copyBoard.setPieceAt(pieceDragging->getPosition(), nullptr);
    copyPiece->setNewPosition(position.first, position.second);
    return copyBoard.isColorInCheck(getColor());
}

bool Player::isCheckMated() {
    // Must be in check AND have no legal moves
    if(!board->isColorInCheck(getColor())) return false;

    for(Move & m: board->getAllMovesForColor(getColor())){
        if(!isInCheck(m.getMovingPiece(), m.getNewPosition())){
            return false;
        }
    }
    return true;
}

bool Player::isStalemated() {
    // Not in check but no legal moves = stalemate (draw)
    if(board->isColorInCheck(getColor())) return false;

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

float Player::getTime() {
    return timeLeft;
}

void Player::setTime(float newTime) {
    this->timeLeft = newTime;
}