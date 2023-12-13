//
// Created by hugh on 12/9/23.
//

#include <utility>

#include "../Header/Board.h"

Board::Board(){
    screen = nullptr;
    running = true;
    renderer = nullptr;
}

int Board::onExecute() {

    if(!OnInit()) {
        return -1;
    }

    SDL_Event Event;

    while(this->running) {
        while(SDL_PollEvent(&Event)) {

            OnEvent(&Event);

        }

        drawBoard();
        SDL_UpdateWindowSurface(screen);


        //OnLoop();
        //OnRender();
    }

    OnCleanup();

    return 0;
}

bool Board::OnInit() {
    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        return false;
    }

    screen = SDL_CreateWindow("ChessPP", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 700, 700, SDL_WINDOW_OPENGL);

    if(screen == nullptr){
        return false;
    }

    renderer = SDL_CreateSoftwareRenderer(SDL_GetWindowSurface(screen));

    if (renderer == nullptr){
        return false;
    }
    return true;
}

void Board::OnEvent(SDL_Event *Event) {
    if(Event->type == SDL_QUIT) {
        running = false;
    }
}

void Board::OnLoop() {

}

void Board::OnRender() {

}

void Board::OnCleanup() {
    SDL_Quit();
}

void Board::drawBoard()  {
    int row = 0, column = 0, x = 0;
    SDL_Rect rect, darea;

    /* Get the Size of drawing surface */
    SDL_RenderGetViewport(renderer, &darea);

    for (; row < 8; row++) {
         column = row % 2;
         x = column;
         for (; column < 4 + (row % 2); column++) {
                 SDL_SetRenderDrawColor(renderer, 1, 55, 32, 0xFF);

                 rect.w = darea.w / 8;
                 rect.h = darea.h / 8;
                 rect.x = x * rect.w;
                 rect.y = row * rect.h;
                 x = x + 2;
                 SDL_RenderFillRect(renderer, &rect);
         }
     }
}


