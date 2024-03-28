//
// Created by John Johnston on 3/28/24.
//

#ifndef CHESSPP_QUEEN_H
#define CHESSPP_QUEEN_H

#include "Piece.h"

class Queen: public Piece{
public:
    Queen();
    Queen(Color color);
    Queen(int row, int col, Color color);
    Queen(pair<int,int> position, Color color);

    vector<Move> getMoves(Board * board) override;
    string getImagePath() override;
};


#endif //CHESSPP_QUEEN_H
