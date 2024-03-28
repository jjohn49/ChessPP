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
    isWhitesTurn = true;
    pieceDragging = nullptr;
    colorsTurn = Piece::White;
}

void Chess::play() {
    //board.print();
    //Move move = Move(1,1,6,1,board.getPieceAt(1,1).get());
    //board.movePiece(Move(1,1,6,1,board.getPieceAt(1,1).get()));
    //std::cout<< move.toString();
    //std::cout << board.getPieceAt(6,1).get()->toString();
    //board.print();
    onExecute();
}

void Chess::drawBoard() {
    SDL_Rect rect, darea;
    SDL_Texture * img = nullptr;
    int w,h;

    /* Get the Size of drawing surface */
    SDL_RenderGetViewport(renderer, &darea);
    bool blackStart = false;

    for (int row = 0; row < 8; row++) {
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
            rect.y = 700 - row * rect.h;

            SDL_RenderFillRect(renderer, &rect);

            std::shared_ptr piece = board.getPieceAt(row,column);
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
    }
    onCleanup();

    return true;
}

void Chess::onCleanup() {
    SDL_Quit();
}

void Chess::onEvent(SDL_Event *event) {
    drawBoard();
    if(event->type == SDL_MOUSEBUTTONDOWN) {
        if(pieceDragging == nullptr){
            setPieceDragging(event);
        }else{
            onPlacePieceDragging(event);
        }

    }else if(event->type == SDL_MOUSEMOTION){
        if(pieceDragging != nullptr){
            onPieceDraggingMoved(event);
        }
    }
    else if(event->type == SDL_QUIT) {
        running = false;
    }
}

void Chess::setPieceDragging(SDL_Event *event) {
    int x, y;
    SDL_GetMouseState(&y, &x);
    std::pair<int,int> pos = std::make_pair(convertYAxisToRow(x), floor((y-200)/100));

    std::shared_ptr<Piece> pieceChosen = board.getPieceAt(pos);
    if(pieceChosen != nullptr && pieceChosen->getColor() == colorsTurn){
        pieceDragging = pieceChosen;
        //vector<Move> moves{pieceDragging.get()->getMoves(&board)};
        board.setPieceAt(pos, nullptr);
    }
}

void Chess::onPieceDraggingMoved(SDL_Event * event) {
    SDL_Rect rect, darea;
    SDL_Texture * img = nullptr;
    int w,h;

    /* Get the Size of drawing surface */
    SDL_RenderGetViewport(renderer, &darea);

    int x, y;
    SDL_GetMouseState(&x,&y);
    img = IMG_LoadTexture(renderer, pieceDragging->getImagePath().c_str());
    SDL_QueryTexture(img, NULL, NULL, &w, &h);

    rect.w = darea.w/8;
    rect.h = darea.h/8;
    rect.x = x -50;
    rect.y = y -50;
    SDL_RenderCopy(renderer, img, NULL, &rect);

    SDL_RenderPresent(renderer);
    SDL_UpdateWindowSurface(screen);

}

void Chess::onPlacePieceDragging(SDL_Event *event) {
    int x, y;
    SDL_GetMouseState(&y,&x);
    //TODO: Need to add valid move code here
    std::pair<int,int> pos = std::make_pair(convertYAxisToRow(x), floor((y-200)/100));

    Move attemptedMove = Move(pieceDragging->getPosition(), pos, pieceDragging, board.getPieceAt(pos));

    if(canPieceMoveThere(attemptedMove)){
        board.movePiece(attemptedMove);
        colorsTurn = (colorsTurn==Piece::White)? Piece::Black : Piece::White;
    }else{
        board.setPieceAt(pieceDragging->getPosition(), pieceDragging);
    }

    pieceDragging.reset();
}

bool Chess::canPieceMoveThere(Move & attemptedMove) {
    for(Move & m: pieceDragging->getMoves(&board)){
        //TODO: Add Method to Check if Move Creates Check

        if(m.getNewPosition() == attemptedMove.getNewPosition() && !isInCheck(attemptedMove.getNewPosition())){
            attemptedMove = m;
            return true;
        }
    }
    return false;
}

int Chess::convertYAxisToRow(int value) {
    return 7 - value/100;
}

bool Chess::isInCheck(std::pair<int,int> position) {
    bool ret;
    Board copyBoard = board;
    copyBoard.setPieceAt(position, pieceDragging);
    pair<int,int> oldPos = pieceDragging->getPosition();
    pieceDragging->setNewPosition(position.first, position.second);
    ret = copyBoard.isColorInCheck(colorsTurn);
    pieceDragging->setNewPosition(oldPos.first,oldPos.second);
    return ret;
}
