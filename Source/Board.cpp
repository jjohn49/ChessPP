//
// Created by hugh on 12/9/23.
//

#include "../Header/Board.h"



Board::Board(){
    screen = NULL;
    this->running = true;
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

    screen = SDL_CreateWindow("ChessPP", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_OPENGL);

    if(screen == nullptr){
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


