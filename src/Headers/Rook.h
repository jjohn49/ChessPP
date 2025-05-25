//
// Created by John Johnston on 3/28/24.
//

#ifndef CHESSPP_ROOK_H
#define CHESSPP_ROOK_H

#include "Piece.h"
#include "Move.h"


class Rook: public Piece{
    vector<vector<float>> evalBoard;
public:
    Rook();
    Rook(int row, int col, Color color);
    Rook(std::pair<int,int> position, Color color);

    vector<Move> getMoves(Board * board) override;
    string getImagePath() override;
    vector<vector<float>> getEvalBoard() override;
};


#endif //CHESSPP_ROOK_H
