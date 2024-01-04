//
// Created by hugh on 12/9/23.
//

#ifndef CHESSPP_BOARD_H
#define CHESSPP_BOARD_H
#include <iostream>
#include <memory>
#include <SDL2/SDL.h>
#include "Piece.h"
#include "Pawn.h"
#include "Rook.h"
#include "Knight.h"
#include "Bishop.h"
#include "Queen.h"
#include "King.h"


class Board {
private:
    std::shared_ptr<Piece> pieceDragging;
    std::vector<std::shared_ptr<Piece>> & captureWhitePieces;
    std::vector<std::shared_ptr<Piece>> & captureBlackPieces;
    std::vector<Move> & allGameMoves;
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

    std::vector<Bishop> whiteBishops{
            Bishop(Piece::White, 0,2),
            Bishop(Piece::White, 0,5)
    };

    std::vector<Bishop> blackBishops{
            Bishop(Piece::Black, 7,2),
            Bishop(Piece::Black, 7,5)
    };

    std::vector<Rook> whiteRooks{
            Rook(Piece::White, 0,0),
            Rook(Piece::White, 0,7)
    };

    std::vector<Rook> blackRooks{
            Rook(Piece::Black, 7,0),
            Rook(Piece::Black, 7,7)
    };

    std::vector<Knight> whiteKnights{
            Knight(Piece::White, 0,1),
            Knight(Piece::White,0,6)
    };

    std::vector<Knight> blackKnights{
            Knight(Piece::Black, 7,1),
            Knight(Piece::Black,7, 6)
    };

    King whiteKing{Piece::White, 0, 4};
    King blackKing{Piece::Black, 7, 4};


    //making these arrays because more can be added, probs should make these vectors
    std::vector<Queen> whiteQueens{Queen(Piece::White, 0,3)};
    std::vector<Queen> blackQueens{Queen(Piece::Black, 7, 3)};

    std::shared_ptr<Piece> board[8][8]{
            {std::make_shared<Rook>(whiteRooks[0]),                               std::make_shared<Knight>(whiteKnights[0]),                               std::make_shared<Bishop>(whiteBishops[0]),                               std::make_shared<Queen>(whiteQueens[0]),                               std::make_shared<King>(whiteKing),                               std::make_shared<Bishop>(whiteBishops[1]),                               std::make_shared<Knight>(whiteKnights[1]),                               std::make_shared<Rook>(whiteRooks[1])},
            {std::make_shared<Pawn>(whitePawns[0]), std::make_shared<Pawn>(whitePawns[1]), std::make_shared<Pawn>(whitePawns[2]), std::make_shared<Pawn>(whitePawns[3]), std::make_shared<Pawn>(whitePawns[4]), std::make_shared<Pawn>(whitePawns[5]), std::make_shared<Pawn>(whitePawns[6]) , std::make_shared<Pawn>(whitePawns[7])},
            {nullptr,                               nullptr, nullptr,                               nullptr,                               nullptr,                               nullptr,                               nullptr,                               nullptr},
            {nullptr,                               nullptr,                               nullptr,                               nullptr,                               nullptr,                               nullptr,                               nullptr,                               nullptr},
            {nullptr,                               nullptr,                               nullptr,                               nullptr,                               nullptr,                               nullptr,                               nullptr,                               nullptr},
            {nullptr,                               nullptr,                               nullptr,                               nullptr,                               nullptr,                               nullptr,                               nullptr,                               nullptr},
            {std::make_shared<Pawn>(blackPawns[0]), std::make_shared<Pawn>(blackPawns[1]), std::make_shared<Pawn>(blackPawns[2]), std::make_shared<Pawn>(blackPawns[3]), std::make_shared<Pawn>(blackPawns[4]), std::make_shared<Pawn>(blackPawns[5]), std::make_shared<Pawn>(blackPawns[6]), std::make_shared<Pawn>(blackPawns[7])},
            {std::make_shared<Rook>(blackRooks[0]),                               std::make_shared<Knight>(blackKnights[0]),                               std::make_shared<Bishop>(blackBishops[0]),                               std::make_shared<Queen>(blackQueens[0]),                               std::make_shared<King>(blackKing),                               std::make_shared<Bishop>(blackBishops[1]),                               std::make_shared<Knight>(blackKnights[1]),                               std::make_shared<Rook>(blackRooks[1])}
    };

    std::shared_ptr<Piece> copyBoard[8][8]{};
    bool running;
    SDL_Window * screen;
    SDL_Renderer * renderer;

public:
    Board();
    Board(std::vector<std::shared_ptr<Piece>> & captureWhitePieces, std::vector<std::shared_ptr<Piece>> & captureBlackPieces, std::vector<Move> & allGameMoves);
    void printBoard();
    void movePiece(Move move);
    int onExecute();
    bool OnInit();
    void OnEvent(SDL_Event* Event);
    void OnCleanup();
    void drawBoard();
    void setPieceClicked(SDL_Event * event);
    void placePiece(SDL_Event * event);
    std::vector<Move> getCastlingMoves(Piece::Color color);

    std::vector<Move> getAllMoves();

    std::shared_ptr<Piece> getPieceAt(std::pair<int, int> pos);

    bool isCheckFor(Piece::Color color);

    std::vector<Move> getMovesFor(Piece::Color color);

    bool contains(std::vector<Move> list, Move val);
    bool contains(std::vector<Move> list, std::pair<int,int> position);

};


#endif //CHESSPP_BOARD_H
