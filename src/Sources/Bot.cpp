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
    }else if(level==SuperEasy){
        move = getSuperEasyMove();
    }else if(level==Easy){
        move = getEasyMove();
    }else if(level==Intermediate){
        move = getIntermediateMove();
    }else if(level==Hard){
        move = getHardMove();
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

Move Bot::getSuperEasyMove() {
    return getAllMoves()[0];
}

Move Bot::getEasyMove() {
    vector<Move> moves{};
    unordered_map<int, vector<Move>> pointsPerMove;


    for(Move & m: this->getAllMoves()){
        if(!isInCheck(m.getMovingPiece(), m.getNewPosition())){
            pointsPerMove[getBoard()->evaluateMove(m)].push_back(m);
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

Move Bot::getIntermediateMove() {
    return getMinMaxMove(2);
}

Move Bot::getHardMove() {
    return getMinMaxMove(4);
}

int Bot::minMaxAlgo(Move move, int depth, int alpha, int beta, bool maximizing) {
    int colorOffset = (move.getMovingPiece()->getColor()==Piece::White)? 1 : -1;
    if(depth == 0){
        return getBoard()->evaluateMove(move) * colorOffset;
    }

    int val;
    if(maximizing){
        val = -100000;
        for(Move & m : getBoard()->getAllMovesForColor((colorOffset == 1)? Piece::Black: Piece::White)){
            int mEval = minMaxAlgo(m,depth-1, alpha, beta, !maximizing);
            val = max(val, mEval);
            alpha = max(alpha, mEval);
            if(beta<=alpha){
                break;
            }
        }
    }else{
        val = 1000000;
        for(Move & m : getBoard()->getAllMovesForColor((colorOffset == 1)? Piece::Black: Piece::White)){
            int mEval = minMaxAlgo(m,depth-1, alpha, beta, !maximizing);
            val = min(val, mEval);
            alpha = max(alpha, mEval);;
            if(beta<=alpha){
                break;
            }
        }
    }

    return val;
}

Move Bot::getMinMaxMove(int depth) {
    unordered_map<int, vector<Move>> pointsPerMove;

    for(Move & m : getAllMoves()){
        pointsPerMove[minMaxAlgo(m,depth,-100000,1000000,(getColor()==Piece::White))].push_back(m);
    }

    vector<int> keys = {};

    for(auto kv: pointsPerMove){
        keys.push_back(kv.first);
    }

    if(getColor()==Piece::White){
        sort(keys.begin(),keys.end(), greater<int>());
    }else {
        sort(keys.begin(), keys.end(), less<int>());
    }

    vector<Move> bestMoves = pointsPerMove[keys[0]];

    return bestMoves[rand()%bestMoves.size()];
}
