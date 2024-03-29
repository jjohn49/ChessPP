//
// Created by John Johnston on 3/29/24.
//

#ifndef CHESSPP_BOT_H
#define CHESSPP_BOT_H

#include "Player.h"

enum BotDifficulty{
    Random,
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

    void move() override;
    Move getRandomMove();
    Move getEasyMove();

};


#endif //CHESSPP_BOT_H
