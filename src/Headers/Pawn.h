//
// Created by John Johnston on 3/21/24.
//

#ifndef CHESSPP_PAWN_H
#define CHESSPP_PAWN_H

#include "Piece.h"
#include "Move.h"
#include <vector>

using namespace std;

class Pawn: public Piece{
public:
    Pawn(int row, int col, Piece::Color color);
    vector<Move> getMoves(Board * board) override;

    optional<Move> getEnPessantMove(Board * board);
    string getImagePath() override;
};


#endif //CHESSPP_PAWN_H
