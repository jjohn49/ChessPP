//
// Created by hugh on 3/21/24.
//

#include "../Headers/Chess.h"
#include <cmath>
#include <algorithm>

//OS Specific Imports
#ifdef __linux__
    #include <SDL2/SDL_image.h>
#elif __APPLE__
    #include <SDL_image.h>
#endif



#define HEIGHT 800
#define WIDTH 1200

Chess::Chess() {
    board = Board();
    screen = nullptr;
    running = true;
    renderer = nullptr;
}

void Chess::play() {
    board.print();
    onExecute();
}

void Chess::drawBoard() {
    SDL_Rect rect, darea;
    SDL_Texture * img = nullptr;
    int w,h;

    /* Get the Size of drawing surface */
    SDL_RenderGetViewport(renderer, &darea);
    bool blackStart = false;

    for (int row = 7; row >= 0; row--) {
        blackStart = !blackStart;
        for (int column = 0; column < 8; column++) {

            //Setting the tile square
            if(blackStart){
                if(column % 2 ==0){
                    SDL_SetRenderDrawColor(renderer, 1, 55, 32, 0xFF);
                }else {
                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0xFF);
                }
            }else {
                if(column % 2 ==0){
                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0xFF);
                }else {
                    SDL_SetRenderDrawColor(renderer, 1, 55, 32, 0xFF);

                }
            }

            rect.w = 100;
            rect.h = 100;
            rect.x = column * rect.w + 200;
            rect.y = row * rect.h;

            SDL_RenderFillRect(renderer, &rect);

            Piece * piece = board.getPieceAt(row,column);
            if(piece != nullptr){
                img = IMG_LoadTexture(renderer, piece->getImagePath().c_str());
                SDL_QueryTexture(img, NULL, NULL, &w, &h);
                SDL_RenderCopy(renderer, img,NULL,&rect);
            }


        }

    }
    SDL_RenderPresent(renderer);
    SDL_UpdateWindowSurface(screen);
}

bool Chess::onInit() {
    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        return false;
    }

    screen = SDL_CreateWindow("ChessPP", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_OPENGL);

    if(screen == nullptr){
        return false;
    }

    renderer = SDL_CreateSoftwareRenderer(SDL_GetWindowSurface(screen));

    if (renderer == nullptr){
        return false;
    }
    return true;
}


bool Chess::onExecute() {
    if(!onInit()) {
        return -1;
    }

    SDL_Event Event;

    while(this->running) {
        while(SDL_PollEvent(&Event)) {

            onEvent(&Event);

        }
        drawBoard();
    }

    onCleanup();

    return true;
}

void Chess::onCleanup() {
    SDL_Quit();
}

void Chess::onEvent(SDL_Event *event) {

    if(event->type == SDL_MOUSEBUTTONDOWN) {

    }else if(event->type ==SDL_MOUSEBUTTONUP){

    }
    else if(event->type == SDL_QUIT) {
        running = false;
    }
}
