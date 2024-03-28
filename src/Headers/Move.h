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

struct CastleMove{
    pair<int, int> oldPosition;
    pair<int, int> newPosition;
};

class Move {

protected:
    pair<int, int> oldPosition;
    pair<int, int> newPosition;
    std::shared_ptr<Piece> movingPiece;
    std::shared_ptr<Piece> pieceCaptured;
    bool isEnPessant;
    CastleMove castleMove;

public:
    Move();
    Move(int oldRow, int oldCol, int newRow, int newCol, std::shared_ptr<Piece> movingPiece, std::shared_ptr<Piece> pieceCaptured = nullptr, bool isEnPessant = false);
    Move(pair<int,int> oldPosition, pair<int,int> newPosition, std::shared_ptr<Piece> movingPiece, std::shared_ptr<Piece> pieceCaptured = nullptr, bool isEnPessant = false);
    pair<int,int> getOldPosition(){return oldPosition;};
    pair<int, int> getNewPosition(){return newPosition;};

    std::shared_ptr<Piece> getMovingPiece(){return movingPiece;};
    std::shared_ptr<Piece> getCapturedPiece(){return pieceCaptured;};
    bool getIsEnPessant(){return isEnPessant;};
    std::string toString();
    void setIsEnPessant(bool value);
    CastleMove getCastleMove();
};


#endif //CHESSPP_MOVE_H
