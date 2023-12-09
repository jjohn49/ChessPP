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

class Chess {
protected:
    Pawn whitePawns[8]{Pawn(Piece::White, 1, 0),
                                 Pawn(Piece::White,1,1),
                                 Pawn(Piece::White,1,2),
                                 Pawn(Piece::White,1,3),
                                 Pawn(Piece::White,1,4),
                                 Pawn(Piece::White,1,5),
                                 Pawn(Piece::White,1,6),
                                 Pawn(Piece::White,1,7)};

    Pawn blackPawns[8]{Pawn(Piece::Black, 6, 0),
                                 Pawn(Piece::Black,6,1),
                                 Pawn(Piece::Black,6,2),
                                 Pawn(Piece::Black,6,3),
                                 Pawn(Piece::Black,6,4),
                                 Pawn(Piece::Black,6,5),
                                 Pawn(Piece::Black,6,6),
                                 Pawn(Piece::Black,6,7)};

    Bishop whiteBishops[2]{
            Bishop(Piece::White, 0,2),
            Bishop(Piece::White, 0,5)
    };

    Bishop blackBishops[2]{
            Bishop(Piece::Black, 7,2),
            Bishop(Piece::Black, 7,5)
    };

    Rook whiteRooks[2]{
            Rook(Piece::White, 0,0),
            Rook(Piece::White, 0,7)
    };

    Rook blackRooks[2]{
            Rook(Piece::Black, 7,0),
            Rook(Piece::Black, 7,7)
    };

    std::vector<std::shared_ptr<Piece>> captureWhitePieces{};
    std::vector<std::shared_ptr<Piece>> captureBlackPieces{};

     std::shared_ptr<Piece> board[8][8]{
            {std::make_shared<Rook>(whiteRooks[0]),                               nullptr,                               std::make_shared<Bishop>(whiteBishops[0]),                               nullptr,                               nullptr,                               std::make_shared<Bishop>(whiteBishops[1]),                               nullptr,                               std::make_shared<Rook>(whiteRooks[1])},
            {std::make_shared<Pawn>(whitePawns[0]), std::make_shared<Pawn>(whitePawns[1]), std::make_shared<Pawn>(whitePawns[2]), std::make_shared<Pawn>(whitePawns[3]), std::make_shared<Pawn>(whitePawns[4]), std::make_shared<Pawn>(whitePawns[5]), std::make_shared<Pawn>(whitePawns[6]), std::make_shared<Pawn>(whitePawns[7])},
            {nullptr,                               nullptr, nullptr,                               nullptr,                               nullptr,                               nullptr,                               nullptr,                               nullptr},
            {nullptr,                               nullptr,                               nullptr,                               nullptr,                               nullptr,                               nullptr,                               nullptr,                               nullptr},
            {nullptr,                               nullptr,                               nullptr,                               nullptr,                               nullptr,                               nullptr,                               nullptr,                               nullptr},
            {nullptr,                               nullptr,                               nullptr,                               nullptr,                               nullptr,                               nullptr,                               nullptr,                               nullptr},
            {std::make_shared<Pawn>(blackPawns[0]), std::make_shared<Pawn>(blackPawns[1]), std::make_shared<Pawn>(blackPawns[2]), std::make_shared<Pawn>(blackPawns[3]), std::make_shared<Pawn>(blackPawns[4]), std::make_shared<Pawn>(blackPawns[5]), std::make_shared<Pawn>(blackPawns[6]), std::make_shared<Pawn>(blackPawns[7])},
            {std::make_shared<Rook>(blackRooks[0]),                               nullptr,                               std::make_shared<Bishop>(blackBishops[0]),                               nullptr,                               nullptr,                               std::make_shared<Bishop>(blackBishops[1]),                               nullptr,                               std::make_shared<Rook>(blackRooks[1])}
    };
public:
    Chess();
    void play();
    void printBoard();
    std::shared_ptr<Piece> getPieceAt(std::pair<int,int> pos);
    void movePiece(Move move);
    std::vector<Move> getAllMoves();
    void printAllMoves();

};


#endif //CHESSPP_CHESS_H
