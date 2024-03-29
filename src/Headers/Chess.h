//
// Created by hugh on 3/21/24.
//

#ifndef CHESSPP_CHESS_H
#define CHESSPP_CHESS_H

#include <iostream>
#include "Board.h"
#include "Player.h"
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
    Player * currentPlayer;
    Player whitePlayer;
    Player blackPlayer;


    void drawBoard();
    void onPieceDraggingMoved(SDL_Event * event);
    void onPlacePieceDragging(SDL_Event * event);
    bool onInit();
    bool onExecute();
    void onEvent(SDL_Event * event);
    static void onCleanup();
    void setPieceDragging(SDL_Event * event);
    int convertYAxisToRow(int row);
    bool isPositionInMoves(int row, int col, vector<Move> & moves);

public:
    Chess();
    void play();
};


#endif //CHESSPP_CHESS_H
