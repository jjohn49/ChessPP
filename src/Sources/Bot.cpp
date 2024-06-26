//
// Created by John Johnston on 3/29/24.
//

#include "../Headers/Bot.h"
#include <cstdlib>
#include <time.h>




Bot::Bot(): Bot(Easy,Piece::NoColor, nullptr)  {}

Bot::Bot(BotDifficulty level, Piece::Color color, Board *board): Player(color, board) {
    this->level = level;
}

bool Bot::isBot() {
    return true;
}

void Bot::move() {
    Move move;
    if(level==Random){
        move = getRandomMove();
    }else if(level==Easy){
        move = getEasyMove();
    }

    this->getBoard()->setPieceAt(move.getOldPosition(), nullptr);
    this->movePiece(move);
}

Move Bot::getRandomMove() {
    vector<Move> moves{};
    for(Move & m: this->getAllMoves()){
        if(!isInCheck(m.getMovingPiece(), m.getNewPosition())){
           moves.push_back(m);
        }
    }

    srand(time(0));
    return moves[rand() % moves.size()];
}

Move Bot::getEasyMove() {
    return getAllMoves()[0];
}
