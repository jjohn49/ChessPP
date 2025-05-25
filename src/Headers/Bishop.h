//
// Created by John Johnston on 3/28/24.
//

#ifndef CHESSPP_BISHOP_H
#define CHESSPP_BISHOP_H

#include "Piece.h"


class Bishop: public Piece{
    vector<vector<float>> evalBoard;
public:
    Bishop();
    Bishop(int row, int col, Color color);
    Bishop(pair<int,int> position, Color color);

    vector<Move> getMoves(Board * board) override;
    string getImagePath() override;
    vector<vector<float>> getEvalBoard() override;

};


#endif //CHESSPP_BISHOP_H
