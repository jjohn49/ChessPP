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

    cout <<"Board evaluated at: "<< getBoard()->evaluate() << endl;
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
    return getMinMaxMove(1);
}

Move Bot::getIntermediateMove() {
    return getMinMaxMove(3);
}

Move Bot::getHardMove() {
    return getMinMaxMove(5);
}

float Bot::minMaxAlgo(Board board, Move & move, int depth, float alpha, float beta, bool maximizing) {

    if(depth == 0){
        return board.evaluate();
    }

    board.setPieceAt(move.getNewPosition(), move.getMovingPiece());
    board.setPieceAt(move.getOldPosition(), nullptr);
    move.getMovingPiece()->setNewPosition(move.getNewPosition().first, move.getNewPosition().second);

    if(move.getIsPawnPromotion()){
        std::shared_ptr<Piece> tempQueen = make_shared<Queen>(Queen(move.getNewPosition(), move.getMovingPiece()->getColor()));
        board.setPieceAt(move.getNewPosition(),tempQueen);
    }

    if(maximizing){
        for(Move & m : board.getAllMovesForColor((move.getMovingPiece()->getColor()==Piece::White)? Piece::Black: Piece::White)){
            if(m.getIsEnPessant()){
                continue;
            }

            float mEval = minMaxAlgo(board, m,depth-1, alpha, beta, false);
            if(mEval >= beta){
                move.getMovingPiece()->setNewPosition(move.getOldPosition().first,move.getOldPosition().second);
                return beta;
            }
            alpha = max(alpha, mEval);
        }
        move.getMovingPiece()->setNewPosition(move.getOldPosition().first,move.getOldPosition().second);
        return alpha;
    }else{
        for(Move & m : board.getAllMovesForColor((move.getMovingPiece()->getColor()==Piece::White)? Piece::Black: Piece::White)){
            if(m.getIsEnPessant()){
                continue;
            }
            float mEval = minMaxAlgo(board, m,depth-1, alpha, beta, true);
            if(mEval <= alpha){
                move.getMovingPiece()->setNewPosition(move.getOldPosition().first,move.getOldPosition().second);
                return alpha;
            }
            beta = min(beta,mEval);
        }
        move.getMovingPiece()->setNewPosition(move.getOldPosition().first,move.getOldPosition().second);
        return beta;
    }
}

Move Bot::getMinMaxMove(int depth) {
    unordered_map<float, vector<Move>> pointsPerMove;

    for(Move & m : getAllMoves()){
        float val = minMaxAlgo(*getBoard(),m,depth,-100000,1000000, true);
        pointsPerMove[val].push_back(m);
    }

    vector<float> keys = {};

    for(auto kv: pointsPerMove){
        keys.push_back(kv.first);
    }

    if(getColor()==Piece::White){
        sort(keys.begin(),keys.end(), greater<float>());
    }else {
        sort(keys.begin(), keys.end(), less<float>());
    }

    vector<Move> bestMoves = pointsPerMove[keys[0]];

    return bestMoves[rand()%bestMoves.size()];
}
