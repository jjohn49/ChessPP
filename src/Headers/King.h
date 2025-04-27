//
// Created by John Johnston on 3/26/24.
//

#ifndef CHESSPP_KING_H
#define CHESSPP_KING_H

#include "Piece.h"

class King: public Piece{
    vector<vector<float>> evalBoard;
public:
    King();
    King(Color color);

    vector<Move> getMoves(Board * board) override;
    vector<Move>getCastlingMoves(Board * board);
    string getImagePath() override;

    vector<vector<float>> getEvalBoard() override;

};


#endif //CHESSPP_KING_H
