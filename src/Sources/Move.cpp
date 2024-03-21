//
// Created by hugh on 3/21/24.
//

#include "../Headers/Move.h"

Move::Move(pair<int, int> oldPosition, pair<int, int> newPosition, Piece * movingPiece) {
    this->oldPosition = oldPosition;
    this->newPosition = newPosition;
    this->movingPiece = movingPiece;
}

Move::Move(int oldRow, int oldCol, int newRow, int newCol, Piece *movingPiece):
Move(make_pair(oldRow, oldCol),make_pair(newRow, newCol), movingPiece) {}

Move::Move(): Move(-1,-1,-1,-1, nullptr) {}
