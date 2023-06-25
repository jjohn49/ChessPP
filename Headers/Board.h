//
// Created by hugh on 6/24/23.
//

#ifndef CHESSPP_BOARD_H
#define CHESSPP_BOARD_H

#include "string"
#include <unordered_map>
using namespace std;

class Board {
    unordered_map<char, int32_t> board;
public:
    Board();

    string toString();
};


#endif //CHESSPP_BOARD_H
