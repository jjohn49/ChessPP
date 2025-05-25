//
// Created by John Johnston on 3/29/24.
//

#ifndef CHESSPP_BOT_H
#define CHESSPP_BOT_H

#include "Player.h"

enum BotDifficulty{
    Random,
    SuperEasy,
    Easy,
    Intermediate,
    Hard,
    Expert
};


class Bot : public Player{
    BotDifficulty level;
public:
    Bot();
    Bot(BotDifficulty level, Piece::Color color, Board * board);
    bool isBot() override;
    bool move() override;
    Move getRandomMove();
    Move getSuperEasyMove();
    Move getEasyMove();
    Move getIntermediateMove();
    Move getMinMaxMove(int depth);
    Move getNegaMaxMove(int depth);
    vector<Move> getAllMovesSorted();
    vector<Move> sortMoves(vector<Move> moves);
    Move getHardMove();
    static float minMaxAlgo(Board board, Move & move, int depth, float alpha, float beta, bool maximizing);
    static float negaMaxAlgo(Board board, Move & move, int depth, float alpha, float beta);
};


#endif //CHESSPP_BOT_H
