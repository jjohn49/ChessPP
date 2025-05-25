//
// Created by John Johnston on 3/28/24.
//

#ifndef CHESSPP_KNIGHT_H
#define CHESSPP_KNIGHT_H

#include "Piece.h"

class Knight: public Piece{
    vector<vector<float>> evalBoard;
public:
    Knight();
    Knight(int row, int col, Color color);
    Knight(pair<int,int> position, Color color);

    vector<Move> getMoves(Board * board) override;
    string getImagePath() override;
    vector<vector<float>> getEvalBoard() override;

};


#endif //CHESSPP_KNIGHT_H
