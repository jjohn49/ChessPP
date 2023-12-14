//
// Created by John Johnston on 12/4/23.
//

#ifndef CHESSPP_CHESS_H
#define CHESSPP_CHESS_H

#include <iostream>
#include <vector>
#include "Pawn.h"
#include <memory>
#include <optional>
#include "Bishop.h"
#include "Rook.h"
#include "Queen.h"
#include "Knight.h"
#include "King.h"
#include "Board.h"

class Chess {
protected:
    std::vector<std::shared_ptr<Piece>> captureWhitePieces{};
    std::vector<std::shared_ptr<Piece>> captureBlackPieces{};
    Board board{};
    std::vector<Move> allGameMoves{};
public:
    Chess();
    void play();
    void printBoard();
    void drawBoard();
    void movePiece(Move move);
};


#endif //CHESSPP_CHESS_H
