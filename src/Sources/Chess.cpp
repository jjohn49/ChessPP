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
    whitePlayer = make_shared<Player>(Player(Piece::White, &board));
    blackPlayer = make_shared<Player>(Player(Piece::Black, &board));
    currentPlayer = whitePlayer;
}

Chess::Chess(bool useBot, BotDifficulty level, Piece::Color botColor){
    board = Board();
    screen = nullptr;
    running = true;
    renderer = nullptr;
    isWhitesTurn = true;
    pieceDragging = nullptr;
    if(useBot){
        whitePlayer = (botColor==Piece::White)? make_shared<Bot>(Bot(level, botColor, &board)): make_shared<Player>(Player(Piece::White, &board));
        blackPlayer = (botColor==Piece::Black)? make_shared<Bot>(Bot(level, botColor,&board)):make_shared<Player>(Player(Piece::Black, &board));
    }
    currentPlayer = whitePlayer;
}

void Chess::play() {
    onExecute();
}

void Chess::drawBoard() {
    SDL_Rect rect, darea;
    SDL_Texture * img = nullptr;
    int w,h;

    /* Get the Size of drawing surface */
    SDL_RenderGetViewport(renderer, &darea);
    bool blackStart = true;

    vector<Move> draggingMoves = {};

    if(pieceDragging != nullptr){
        for(Move & m: pieceDragging->getMoves(&board)){
            if(!currentPlayer->isInCheck(pieceDragging,m.getNewPosition())){
                draggingMoves.push_back(m);
            }
        }
    }

    for (int row = 0; row < 8; row++) {
        for (int column = 0; column < 8; column++) {

            if(isPositionInMoves(row,column,draggingMoves)){
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0xFF);
            }
            //Setting the tile square
            else if(blackStart){
                SDL_SetRenderDrawColor(renderer, 1, 55, 32, 0xFF);
            }else {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0xFF);
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
            blackStart = !blackStart;
        }
        blackStart = !blackStart;
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
        running = !currentPlayer->isCheckMated();
        while(running && SDL_PollEvent(&Event)) {
            onEvent(&Event);
        }
    }
    std::string winner = (currentPlayer->getColor()==Piece::White)? "Black": "White";
    std::cout << winner << " won" << std::endl;
    onCleanup();

    return true;
}

void Chess::onCleanup() {
    SDL_Quit();
}

void Chess::onEvent(SDL_Event *event) {
    if(event->type == SDL_QUIT) {
        running = false;
    }

    drawBoard();

    if(!currentPlayer->isBot()){
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
    }else{
        currentPlayer->move();
        currentPlayer = (currentPlayer->getColor()==Piece::White)? blackPlayer:whitePlayer;
        std::cout<<"Hello";
    }


}

void Chess::setPieceDragging(SDL_Event *event) {
    int x, y;
    SDL_GetMouseState(&y, &x);
    std::pair<int,int> pos = std::make_pair(convertYAxisToRow(x), floor((y-200)/100));

    std::shared_ptr<Piece> pieceChosen = board.getPieceAt(pos);
    if(pieceChosen != nullptr && pieceChosen->getColor() == currentPlayer->getColor()){
        pieceDragging = pieceChosen;
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

    if(currentPlayer->movePiece(attemptedMove)){
        currentPlayer = (currentPlayer->getColor()==Piece::White)? blackPlayer:whitePlayer;
    }

    pieceDragging.reset();
}

int Chess::convertYAxisToRow(int value) {
    return 7 - value/100;
}

bool Chess::isPositionInMoves(int row, int col, vector<Move> &moves) {
    for(Move & m: moves){
        if(m.getNewPosition() == make_pair(row,col)){
            return true;
        }
    }

    return false;
}

void Chess::onWon() {

}

void Chess::onPawnPromotion() {

}



