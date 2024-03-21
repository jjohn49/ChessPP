//
// Created by hugh on 3/21/24.
//

#ifndef CHESSPP_CHESS_H
#define CHESSPP_CHESS_H

#include <iostream>
#include "Board.h"


class Chess {
protected:
    Board board;

public:
    Chess();
    void play();
};


#endif //CHESSPP_CHESS_H
