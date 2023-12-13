//
// Created by hugh on 12/9/23.
//

#ifndef CHESSPP_BOARD_H
#define CHESSPP_BOARD_H
#include <iostream>
#include <memory>
#include <SDL2/SDL.h>

class Board {
private:
    bool running;
    SDL_Window * screen;

public:
    Board();
    int onExecute();
    bool OnInit();
    void OnEvent(SDL_Event* Event);
    void OnLoop();
    void OnRender();
    void OnCleanup();
};


#endif //CHESSPP_BOARD_H
