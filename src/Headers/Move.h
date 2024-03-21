//
// Created by hugh on 3/21/24.
//

#ifndef CHESSPP_MOVE_H
#define CHESSPP_MOVE_H

#include <iostream>
#include <optional>

using namespace std;

//forward declare
class Piece;

class Move {

protected:
    pair<int, int> oldPosition;
    pair<int, int> newPosition;
    Piece * movingPiece;
    bool isEnPessant;
    bool isCastling;

public:
    Move();
    Move(int oldRow, int oldCol, int newRow, int newCol, Piece * movingPiece);
    Move(pair<int,int> oldPosition, pair<int,int> newPosition, Piece * movingPiece);
    pair<int,int> getOldPosition(){return oldPosition;};
    pair<int, int> getNewPosition(){return newPosition;};
};


#endif //CHESSPP_MOVE_H
