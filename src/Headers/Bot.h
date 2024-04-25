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
    Move getHardMove();
    float minMaxAlgo(Board board, Move & move, int depth, float alpha, float beta, bool maximizing);
};


#endif //CHESSPP_BOT_H
