//
// Created by John Johnston on 12/4/23.
//

#ifndef CHESSPP_CHESS_H
#define CHESSPP_CHESS_H

#include <iostream>
#include <vector>
#include "Pieces/Pawn.h"
#include <memory>
#include <optional>
#include "Pieces/Bishop.h"
#include "Pieces/Rook.h"
#include "Pieces/Queen.h"
#include "Pieces/Knight.h"
#include "Pieces/King.h"
#include "Board.h"

class Chess {
protected:
    std::vector<std::shared_ptr<Piece>> captureWhitePieces{};
    std::vector<std::shared_ptr<Piece>> captureBlackPieces{};
    std::vector<Move> allGameMoves{};
    Board board{captureWhitePieces, captureBlackPieces, allGameMoves};

public:
    Chess();
    void play();
    void printBoard();
    void drawBoard();
    void movePiece(Move move);
};


#endif //CHESSPP_CHESS_H
