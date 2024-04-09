//
// Created by John Johnston on 3/29/24.
//

#include "../Headers/Bot.h"
#include <cstdlib>
#include <time.h>
#include "../Headers/Board.h"
#include <unordered_map>




Bot::Bot(): Bot(Easy,Piece::NoColor, nullptr)  {}

Bot::Bot(BotDifficulty level, Piece::Color color, Board *board): Player(color, board) {
    this->level = level;
}

bool Bot::isBot() {
    return true;
}

bool Bot::move() {
    Move move;
    if(level==Random){
        move = getRandomMove();
    }else if(level==Easy){
        move = getEasyMove();
    }else if(level==Intermediate){
        move = getIntermediateMove();
    }

    this->getBoard()->setPieceAt(move.getOldPosition(), nullptr);
    return this->movePiece(move);
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

Move Bot::getIntermediateMove() {
    vector<Move> moves{};
    unordered_map<int, vector<Move>> pointsPerMove;

    unordered_map<Piece::Type, int> pointsPerPiece = {
            {Piece::Pawn,10},
            {Piece::Knight,30},
            {Piece::Bishop,30},
            {Piece::Rook,50},
            {Piece::Queen,90},
            {Piece::King,900},
    };

    for(Move & m: this->getAllMoves()){
        if(!isInCheck(m.getMovingPiece(), m.getNewPosition())){
            if(m.getCapturedPiece()!= nullptr){
                pointsPerMove[pointsPerPiece[m.getCapturedPiece()->getType()]].push_back(m);
            }else{
                pointsPerMove[0].push_back(m);
            }
        }
    }

    vector<int> keys = {};

    for(auto kv: pointsPerMove){
        keys.push_back(kv.first);
    }

    sort(keys.begin(),keys.end(), greater<int>());

    vector<Move> bestMoves = pointsPerMove[keys[0]];

    return bestMoves[rand()%bestMoves.size()];

}
