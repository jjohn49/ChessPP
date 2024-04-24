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
    vector<Move> moves = getAllMoves();

    srand(time(0));
    return moves[rand() % moves.size()];
}

Move Bot::getSuperEasyMove() {
    return getAllMoves()[0];
}

Move Bot::getEasyMove() {
    return getMinMaxMove(0);
}

Move Bot::getIntermediateMove() {
    return getMinMaxMove(2);
}

Move Bot::getHardMove() {
    return getMinMaxMove(3);
}

int Bot::minMaxAlgo(Board board, Move & move, int depth, int alpha, int beta, bool maximizing) {

    if(depth == 0){
        return board.evaluate();
    }

    board.setPieceAt(move.getNewPosition(), move.getMovingPiece());
    move.getMovingPiece()->setNewPosition(move.getNewPosition().first, move.getNewPosition().second);

    int val;
    if(maximizing){
        val = -100000;
        for(Move & m : getBoard()->getAllMovesForColor((move.getMovingPiece()->getColor()==Piece::White)? Piece::Black: Piece::White)){
            int mEval = minMaxAlgo(board, m,depth-1, alpha, beta, false);
            val = max(val, mEval);
            if(val > beta){
                break;
            }
            alpha = max(alpha, mEval);
        }
    }else{
        val = 1000000;
        for(Move & m : getBoard()->getAllMovesForColor((move.getMovingPiece()->getColor()==Piece::White)? Piece::Black: Piece::White)){
            int mEval = minMaxAlgo(board, m,depth-1, alpha, beta, true);
            val = min(val, mEval);
            if(val < alpha){
                break;
            }
            beta = min(beta,val);
        }
    }

    move.getMovingPiece()->setNewPosition(move.getOldPosition().first,move.getOldPosition().second);

    return val;
}

Move Bot::getMinMaxMove(int depth) {
    unordered_map<int, vector<Move>> pointsPerMove;

    for(Move & m : getAllMoves()){
        int val = minMaxAlgo(*getBoard(),m,depth,-100000,1000000,(getColor()==Piece::White));
        pointsPerMove[val].push_back(m);
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
