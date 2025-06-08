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

double Bot::maxAlphaBeta(Board board, Move &move, int depth, double alpha, double beta) {
    try{
        if(depth == 0){
            return board.evaluate();
        }

        double bestVal = -1000000000000;

        Board copyBoard;
        shared_ptr<Piece> movingPiece;

        try {
            copyBoard = board.deepCopy();
            movingPiece = copyBoard.getPieceAt(move.getOldPosition());

            copyBoard.setPieceAt(move.getNewPosition(), movingPiece);
            copyBoard.setPieceAt(move.getOldPosition(), nullptr);
            movingPiece->setHasMoved(true);
            movingPiece->setNewPosition(move.getNewPosition().first, move.getNewPosition().second);
        }catch (...){
            cerr << "Error when creating copy boards" << endl;
            rethrow_exception(current_exception());
        }

        try {

            if (move.getIsPawnPromotion()) {
                std::shared_ptr<Piece> tempQueen = make_shared<Queen>(
                        Queen(move.getNewPosition(), move.getMovingPiece()->getColor()));
                copyBoard.setPieceAt(move.getNewPosition(), tempQueen);
            }

        }catch (...){
            cerr << "Error during pawn promotion to queen" << endl;
            rethrow_exception(current_exception());
        }

        for(Move & m : board.getAllMovesForColor((move.getMovingPiece()->getColor()==Piece::White)? Piece::Black: Piece::White)){
            try {
                double score = minAlphaBeta(copyBoard, m, depth - 1, alpha, beta);

                if (score > bestVal) {
                    bestVal = alpha;
                    if (score > alpha) {
                        alpha = score;
                    }
                }

                if (score >= beta) {
                    return score;
                }
            }catch (...){
                cerr << "Error during loop in minMaxAlpha" << endl;
                cerr << "Move is : " << m.toString();
                rethrow_exception(current_exception());
            }
        }
        return bestVal;
    }catch (...){
        exception_ptr ex = current_exception();
        std::cerr << "Error in minAlphaBeta for depth: " << depth << ", alpha: " << alpha << ", beta: " << beta << "\n"
                  << "Board: \n" << board.toString() << "\n"
                  << "Move: " << move.toString() << endl;
        rethrow_exception(ex);
    }
}

double Bot::minAlphaBeta(Board board, Move &move, int depth, double alpha, double beta) {
    try {
        if (depth == 0) {
            return board.evaluate();
        }

        int bestVal = 1000000000000;

        Board copyBoard;
        shared_ptr<Piece> movingPiece = nullptr;

        try {
            copyBoard = board.deepCopy();
            movingPiece = copyBoard.getPieceAt(move.getOldPosition().first, move.getOldPosition().second);

            if(movingPiece == nullptr){
                cerr << "Moving piece that is suppose to be at " << move.getOldPosition().first << "," << move.getOldPosition().second << " was found to be null" << endl;
                throw exception();
            }

            copyBoard.setPieceAt(move.getNewPosition(), movingPiece);
            copyBoard.setPieceAt(move.getOldPosition(), nullptr);
            movingPiece->setHasMoved(true);
            movingPiece->setNewPosition(move.getNewPosition().first, move.getNewPosition().second);
        }catch (...){
            cerr << "Error when creating copy boards" << endl;
            rethrow_exception(current_exception());
        }

        try {

            if (move.getIsPawnPromotion()) {
                std::shared_ptr<Piece> tempQueen = make_shared<Queen>(
                        Queen(move.getNewPosition(), move.getMovingPiece()->getColor()));
                copyBoard.setPieceAt(move.getNewPosition(), tempQueen);
            }

        }catch (...){
            cerr << "Error during pawn promotion to queen" << endl;
            rethrow_exception(current_exception());
        }

        for (Move &m: copyBoard.getAllMovesForColor(
                (move.getMovingPiece()->getColor() == Piece::White) ? Piece::Black : Piece::White)) {
            try{
                double score = maxAlphaBeta(copyBoard, m, depth - 1, alpha, beta);

                if (score < bestVal) {
                    bestVal = score;
                    if (score < beta) {
                        beta = score;
                    }
                }

                if (score <= alpha) {
                    return score;
                }
            }catch (...){
                cerr << "Error during loop in minMaxAlpha" << endl;
                cerr << "Move is : " << m.toString();
                rethrow_exception(current_exception());
            }
        }
        return bestVal;
    }catch (...){
        exception_ptr ex = current_exception();
        std::cerr << "Error in minAlphaBeta for depth: " << depth << ", alpha: " << alpha << ", beta: " << beta << "\n"
        << "Board: \n" << board.toString() << "\n"
        << "Move: " << move.toString() << endl;
        rethrow_exception(ex);
    }
}

/**Deprecated */
float Bot::minMaxAlgo(Board board, Move & move, int depth, float alpha, float beta, bool maximizing) {

    if(depth == 0){
        return board.evaluate();
    }

    Board copyBoard = board.deepCopy();
    shared_ptr<Piece> movingPiece = copyBoard.getPieceAt(move.getOldPosition());

    copyBoard.setPieceAt(move.getNewPosition(), movingPiece);
    copyBoard.setPieceAt(move.getOldPosition(), nullptr);
    bool tempHasMoved = movingPiece->getHasMoved();
    movingPiece->setHasMoved(true);
    movingPiece->setNewPosition(move.getNewPosition().first, move.getNewPosition().second);

    if(move.getIsPawnPromotion()){
        std::shared_ptr<Piece> tempQueen = make_shared<Queen>(Queen(move.getNewPosition(), move.getMovingPiece()->getColor()));
        copyBoard.setPieceAt(move.getNewPosition(),tempQueen);
    }

    for(Move & m : board.getAllMovesForColor((move.getMovingPiece()->getColor()==Piece::White)? Piece::Black: Piece::White)){
        if(m.getIsEnPessant()){
            //move.getMovingPiece()->setNewPosition(move.getOldPosition().first, move.getOldPosition().second);
            continue;
        }

        float mEval = minMaxAlgo(board, m,depth-1, alpha, beta, !maximizing);

        if(maximizing) {

            if (mEval >= beta) {
                movingPiece->setNewPosition(move.getOldPosition().first, move.getOldPosition().second);
                movingPiece->setHasMoved(tempHasMoved);
                return beta;
            }
            alpha = max(alpha, mEval);
        }else{
            if(mEval <= alpha){
                movingPiece->setNewPosition(move.getOldPosition().first,move.getOldPosition().second);
                movingPiece->setHasMoved(tempHasMoved);
                return alpha;
            }
            beta = min(beta,mEval);
        }
    }
    movingPiece->setNewPosition(move.getOldPosition().first,move.getOldPosition().second);
    movingPiece->setHasMoved(tempHasMoved);
    return maximizing? alpha : beta;
}

float Bot::negaMaxAlgo(Board board, Move &move, int depth, float alpha, float beta) {
    if (depth == 0) {
        return board.evaluate();
    }

    board.setPieceAt(move.getNewPosition(), move.getMovingPiece());
    board.setPieceAt(move.getOldPosition(), nullptr);
    move.getMovingPiece()->setNewPosition(move.getNewPosition().first, move.getNewPosition().second);

    if (move.getIsPawnPromotion()) {
        std::shared_ptr<Piece> tempQueen = make_shared<Queen>(
                Queen(move.getNewPosition(), move.getMovingPiece()->getColor()));
        board.setPieceAt(move.getNewPosition(), tempQueen);
    } else if (move.getIsQueenSideCastle()) {
        int row = (move.getMovingPiece()->getColor() == Piece::White) ? 0 : 7;
        std::shared_ptr<Piece> tempRook = make_shared<Rook>(row, 3, move.getMovingPiece()->getColor());
        board.setPieceAt(move.getNewPosition(), tempRook);
        board.setPieceAt(row, 7, nullptr);
    } else if (move.getIsKingSideCastle()) {
        int row = (move.getMovingPiece()->getColor() == Piece::White) ? 0 : 7;
        std::shared_ptr<Piece> tempRook = make_shared<Rook>(row, 5, move.getMovingPiece()->getColor());
        board.setPieceAt(move.getNewPosition(), tempRook);
        board.setPieceAt(row, 0, nullptr);
    }


    float val = -10000000;
    for (Move &m: board.getAllMovesForColor(
            (move.getMovingPiece()->getColor() == Piece::White) ? Piece::Black : Piece::White)) {
        if (m.getIsEnPessant()) {
            continue;
        }

        float mEval = -negaMaxAlgo(board, m, depth - 1, -beta, -alpha);
        val = max(mEval, val);
        alpha = max(alpha, val);
        if (alpha >= beta) {
            move.getMovingPiece()->setNewPosition(move.getOldPosition().first, move.getOldPosition().second);
            return beta;
        }

    }
    move.getMovingPiece()->setNewPosition(move.getOldPosition().first, move.getOldPosition().second);
    return val;
}

Move Bot::getMinMaxMove(int depth) {
    unordered_map<double, vector<Move>> pointsPerMove;
    vector<pair<Move, future<double>>> futures{};

    for(Move & m : getAllMovesSorted()){
        Board copyBoard = this->getBoard()->deepCopy();
        Move copyMove = m;

        futures.push_back(make_pair(m, std::async(std::launch::async, [copyBoard, &copyMove, depth] {
            double score = maxAlphaBeta(copyBoard, copyMove, depth, -100000000000000, 1000000000000000);
            return double(-1) * score;
        })));

    }

    for(pair<Move, future<double>> & f: futures){

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

    vector<Move> bestMoves = pointsPerMove.at(keys.at(0));

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
