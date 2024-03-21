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

    void drawBoard();
    bool onInit();
    bool onExecute();
    void onEvent(SDL_Event * event);
    static void onCleanup();

public:
    Chess();
    void play();
};


#endif //CHESSPP_CHESS_H
