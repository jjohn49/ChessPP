//
// Created by John Johnston on 3/21/24.
//

#include "../Headers/Pawn.h"

Pawn::Pawn(int row, int col, Piece::Color color): Piece(row, col, color, Piece::Type::Pawn) {}

vector<Move> Pawn::getMoves(Board *board) {
    return Piece::getMoves(board);
}
