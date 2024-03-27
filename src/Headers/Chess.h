//
// Created by hugh on 3/21/24.
//

#ifndef CHESSPP_CHESS_H
#define CHESSPP_CHESS_H

#include <iostream>
#include "Board.h"
#ifdef __linux__
    #include <SDL2/SDL.h>
#elif __APPLE__
    #include <SDL.h>
#endif


class Chess {
protected:
    Board board;
    bool running;
    SDL_Window * screen;
    SDL_Renderer * renderer;
    bool isWhitesTurn;
    std::shared_ptr<Piece> pieceDragging;
    Piece::Color colorsTurn;
    std::vector<Move> moves;

    void drawBoard();
    void onPieceDraggingMoved(SDL_Event * event);
    void onPlacePieceDragging(SDL_Event * event);
    bool onInit();
    bool onExecute();
    void onEvent(SDL_Event * event);
    static void onCleanup();
    void setPieceDragging(SDL_Event * event);
    bool canPieceMoveThere(Move attemptedMove);
    int convertYAxisToRow(int row);
    bool isInCheck(std::pair<int,int> position);

public:
    Chess();
    void play();
};


#endif //CHESSPP_CHESS_H
