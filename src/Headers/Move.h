//
// Created by hugh on 3/21/24.
//

#ifndef CHESSPP_MOVE_H
#define CHESSPP_MOVE_H

#include <iostream>
#include <optional>

using namespace std;

//forward declare Piece
class Piece;

class Move {

protected:
    pair<int, int> oldPosition;
    pair<int, int> newPosition;
    Piece * movingPiece;
    std::shared_ptr<Piece> pieceCaptured;

    bool isEnPessant;

public:
    Move();
    Move(int oldRow, int oldCol, int newRow, int newCol, Piece * movingPiece);
    Move(pair<int,int> oldPosition, pair<int,int> newPosition, Piece * movingPiece);
    pair<int,int> getOldPosition(){return oldPosition;};
    pair<int, int> getNewPosition(){return newPosition;};

    Piece & getMovingPiece(){return *movingPiece;};
    std::shared_ptr<Piece> getCapturedPiece(){return pieceCaptured;};
    bool getIsEnPessant(){return isEnPessant;};
    std::string toString();
};


#endif //CHESSPP_MOVE_H
