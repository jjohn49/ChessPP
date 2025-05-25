//
// Created by John Johnston on 3/29/24.
//

#include "../Headers/Bot.h"
#include <cstdlib>
#include <time.h>
#include "../Headers/Board.h"
#include <unordered_map>
#include <algorithm>
#include <future>




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
    //return getMinMaxMove(2);
    return getNegaMaxMove(2);
}

Move Bot::getHardMove() {
    return getMinMaxMove(4);
    //return getNegaMaxMove(4);
}

vector<Move> Bot::getAllMovesSorted() {
    vector<Move> moves = getAllMoves();

    unordered_map<Piece::Type, int> pointsPerPiece = {
            {Piece::Pawn,1},
            {Piece::Knight,3},
            {Piece::Bishop,3},
            {Piece::Rook,5},
            {Piece::Queen,9},
            {Piece::King,20},
    };

    unordered_map<float, vector<Move>> pointsPerMove;

    for(Move & m: moves){
        float total = 0;
        if(m.getIsPawnPromotion()){
            total += 9;
        }

        if(m.getCapturedPiece()!= nullptr){
            total += pointsPerPiece[m.getCapturedPiece()->getType()];
        }

        if(m.getIsKingSideCastle() || m.getIsQueenSideCastle()){
            total += 2;
        }

        pair<int, int> position = m.getNewPosition();
        total += m.getMovingPiece()->getEvalBoard()[position.first][position.second];

        pointsPerMove[total].push_back(m);
    }

    vector<float> keys = {};

    for(auto kv: pointsPerMove){
        keys.push_back(kv.first);
    }

    sort(keys.begin(),keys.end(), less<float>());

    vector<Move> ret = {};

    if(keys.size() > 0){
        ret = pointsPerMove[keys[0]];
        for(int x = 1; x < keys.size(); x++){
            ret.insert(ret.end(), pointsPerMove[keys[x]].begin(),pointsPerMove[keys[x]].end());
        }
    }

    return ret;
}

float Bot::minMaxAlgo(Board board, Move & move, int depth, float alpha, float beta, bool maximizing) {

    Board copyBoard = board.deepCopy();

    if(depth == 0){
        return board.evaluate();
    }

    board.setPieceAt(move.getNewPosition(), move.getMovingPiece());
    board.setPieceAt(move.getOldPosition(), nullptr);
    bool tempHasMoved = move.getMovingPiece()->getHasMoved();
    move.getMovingPiece()->setHasMoved(true);
    move.getMovingPiece()->setNewPosition(move.getNewPosition().first, move.getNewPosition().second);

    if(move.getIsPawnPromotion()){
        std::shared_ptr<Piece> tempQueen = make_shared<Queen>(Queen(move.getNewPosition(), move.getMovingPiece()->getColor()));
        board.setPieceAt(move.getNewPosition(),tempQueen);
    }

    for(Move & m : board.getAllMovesForColor((move.getMovingPiece()->getColor()==Piece::White)? Piece::Black: Piece::White)){
        if(m.getIsEnPessant()){
            continue;
        }

        float mEval = minMaxAlgo(board, m,depth-1, alpha, beta, !maximizing);

        if(maximizing) {
            if (mEval >= beta) {
                move.getMovingPiece()->setNewPosition(move.getOldPosition().first, move.getOldPosition().second);
                return beta;
            }
            alpha = max(alpha, mEval);
        }else{
            if(mEval <= alpha){
                move.getMovingPiece()->setNewPosition(move.getOldPosition().first,move.getOldPosition().second);
                return alpha;
            }
            beta = min(beta,mEval);
        }
    }
    move.getMovingPiece()->setNewPosition(move.getOldPosition().first,move.getOldPosition().second);
    move.getMovingPiece()->setHasMoved(tempHasMoved);
    return maximizing? alpha : beta;
}

float Bot::negaMaxAlgo(Board board, Move &move, int depth, float alpha, float beta) {
    if(depth==0){
        return board.evaluate();
    }

    board.setPieceAt(move.getNewPosition(), move.getMovingPiece());
    board.setPieceAt(move.getOldPosition(), nullptr);
    move.getMovingPiece()->setNewPosition(move.getNewPosition().first, move.getNewPosition().second);

    if(move.getIsPawnPromotion()){
        std::shared_ptr<Piece> tempQueen = make_shared<Queen>(Queen(move.getNewPosition(), move.getMovingPiece()->getColor()));
        board.setPieceAt(move.getNewPosition(),tempQueen);
    }else if(move.getIsQueenSideCastle()){
        int row = (move.getMovingPiece()->getColor()==Piece::White)? 0 : 7;
        std::shared_ptr<Piece> tempRook = make_shared<Rook>(row, 3, move.getMovingPiece()->getColor());
        board.setPieceAt(move.getNewPosition(),tempRook);
        board.setPieceAt(row,7, nullptr);
    }else if(move.getIsKingSideCastle()){
        int row = (move.getMovingPiece()->getColor()==Piece::White)? 0 : 7;
        std::shared_ptr<Piece> tempRook = make_shared<Rook>(row, 5, move.getMovingPiece()->getColor());
        board.setPieceAt(move.getNewPosition(),tempRook);
        board.setPieceAt(row,0, nullptr);
    }


    float val = -10000000;
    for(Move & m : board.getAllMovesForColor((move.getMovingPiece()->getColor()==Piece::White)? Piece::Black: Piece::White)){
        if(m.getIsEnPessant()){
            continue;
        }

        float mEval = -negaMaxAlgo(board,m,depth-1,-beta,-alpha);
        val = max(mEval,val);
        alpha = max(alpha, val);
        if(alpha >= beta){
            move.getMovingPiece()->setNewPosition(move.getOldPosition().first,move.getOldPosition().second);
            return beta;
        }

    }
    move.getMovingPiece()->setNewPosition(move.getOldPosition().first,move.getOldPosition().second);
    return val;
}

Move Bot::getMinMaxMove(int depth) {
    unordered_map<float, vector<Move>> pointsPerMove;
    vector<pair<Move, future<float>>> futures{};


    for(Move & m : getAllMovesSorted()){
        //float val = minMaxAlgo(*getBoard(),m,depth,-100000,1000000, true);
        Board copyBoard = this->getBoard()->deepCopy();
        Move copyMove = m;
        copyMove.setMovingPiece(copyBoard.getPieceAt(m.getOldPosition()));

        futures.push_back(make_pair(m ,std::async(launch::async, [copyBoard, &copyMove, depth]{
            return minMaxAlgo(copyBoard, copyMove,depth, -100000, 100000, true);
        })));
    }

    for(pair<Move, future<float>> & f: futures){
        float val = f.second.get();
        pointsPerMove[val].push_back(f.first);
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

Move Bot::getNegaMaxMove(int depth) {
    unordered_map<float, vector<Move>> pointsPerMove;

    for(Move & m : getAllMovesSorted()){
        float val = negaMaxAlgo(*getBoard(),m,depth,-100000,1000000);
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

vector<Move> Bot::sortMoves(vector<Move> moves) {

    unordered_map<Piece::Type, int> pointsPerPiece = {
            {Piece::Pawn,1},
            {Piece::Knight,3},
            {Piece::Bishop,3},
            {Piece::Rook,5},
            {Piece::Queen,9},
            {Piece::King,20},
    };

    unordered_map<float, vector<Move>> pointsPerMove;

    for(Move & m: moves){
        float total = 0;
        if(m.getIsPawnPromotion()){
            total += 9;
        }

        if(m.getCapturedPiece()!= nullptr){
            total += pointsPerPiece[m.getCapturedPiece()->getType()];
        }

        if(m.getIsKingSideCastle() || m.getIsQueenSideCastle()){
            total += 2;
        }

        pair<int, int> position = m.getNewPosition();
        total += m.getMovingPiece()->getEvalBoard()[position.first][position.second];

        pointsPerMove[total].push_back(m);
    }

    vector<float> keys = {};

    for(auto kv: pointsPerMove){
        keys.push_back(kv.first);
    }

    sort(keys.begin(),keys.end(), less<float>());

    vector<Move> ret = {};

    if(keys.size() > 0){
        ret = pointsPerMove[keys[0]];
        for(int x = 1; x < keys.size(); x++){
            ret.insert(ret.end(), pointsPerMove[keys[x]].begin(),pointsPerMove[keys[x]].end());
        }
    }

    return ret;
}
