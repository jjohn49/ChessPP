//
// Created by hugh on 3/21/24.
//

#include "../Headers/Chess.h"
#include <cmath>
#include <algorithm>
#include <unistd.h>

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
    pieceDragging = nullptr;
    whitePlayer = make_shared<Player>(Player(Piece::White, &board));
    blackPlayer = make_shared<Player>(Player(Piece::Black, &board));
    currentPlayer = whitePlayer;
    this->currentTime = chrono::steady_clock::now();
}

Chess::Chess(bool useBot, BotDifficulty level, Piece::Color botColor){
    board = Board();
    screen = nullptr;
    running = true;
    renderer = nullptr;
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

void Chess::drawTime() {
    SDL_Rect dest;
    dest.x = 0;
    dest.y = 350;
    dest.w = 200;
    dest.h = 200;

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF);
    SDL_RenderFillRect(renderer,&dest);

    dest.h = 100;
    dest.w = 200;

    SDL_Color color{255,255,255};
    SDL_Surface * surface = TTF_RenderText_Solid(font, to_string(whitePlayer->getTime()).c_str(), color);
    SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_RenderCopy(renderer, texture, NULL, &dest);
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);


    dest.x = 1000;
    dest.y = 350;
    dest.w = 200;
    dest.h = 200;

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF);
    SDL_RenderFillRect(renderer,&dest);

    dest.h = 100;
    dest.w = 200;

    surface = TTF_RenderText_Solid(font, to_string(blackPlayer->getTime()).c_str(), color);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_RenderCopy(renderer, texture, NULL, &dest);
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);

    dest.y+=90;


    SDL_RenderPresent(renderer);
    SDL_UpdateWindowSurface(screen);
}

void Chess::drawBoard() {
    SDL_Rect rect, darea;
    SDL_Texture * img = nullptr;
    int w,h;

    /* Get the Size of drawing surface */
    SDL_RenderGetViewport(renderer, &darea);
    bool blackStart = true;

    //this->drawTime();

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
                img = pieceTextures[piece->getType()][piece->getColor()];
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

    if(TTF_Init()<0){
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

    font = TTF_OpenFont("../assets/fonts/arial.ttf",25);



    //preload all textures
    pieceTextures[Piece::Pawn] = {{Piece::White,IMG_LoadTexture(renderer, string("../assets/PNGs/No shadow/2x/w_pawn_2x_ns.png").c_str())},
                                  {Piece::Black,IMG_LoadTexture(renderer, string("../assets/PNGs/No shadow/2x/b_pawn_2x_ns.png").c_str())}};

    pieceTextures[Piece::Bishop] = {{Piece::White,IMG_LoadTexture(renderer, string("../assets/PNGs/No shadow/2x/w_bishop_2x_ns.png").c_str())},
                                    {Piece::Black,IMG_LoadTexture(renderer, string("../assets/PNGs/No shadow/2x/b_bishop_2x_ns.png").c_str())}};

    pieceTextures[Piece::Knight] = {{Piece::White,IMG_LoadTexture(renderer, string("../assets/PNGs/No shadow/2x/w_knight_2x_ns.png").c_str())},
                                    {Piece::Black,IMG_LoadTexture(renderer, string("../assets/PNGs/No shadow/2x/b_knight_2x_ns.png").c_str())}};

    pieceTextures[Piece::Rook] = {{Piece::White,IMG_LoadTexture(renderer, string("../assets/PNGs/No shadow/2x/w_rook_2x_ns.png").c_str())},
                                  {Piece::Black,IMG_LoadTexture(renderer, string("../assets/PNGs/No shadow/2x/b_rook_2x_ns.png").c_str())}};

    pieceTextures[Piece::Queen] = {{Piece::White,IMG_LoadTexture(renderer, string("../assets/PNGs/No shadow/2x/w_queen_2x_ns.png").c_str())},
                                   {Piece::Black,IMG_LoadTexture(renderer, string("../assets/PNGs/No shadow/2x/b_queen_2x_ns.png").c_str())}};

    pieceTextures[Piece::King] = {{Piece::White,IMG_LoadTexture(renderer, string("../assets/PNGs/No shadow/2x/w_king_2x_ns.png").c_str())},
                                  {Piece::Black,IMG_LoadTexture(renderer, string("../assets/PNGs/No shadow/2x/b_king_2x_ns.png").c_str())}};

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

    updateTime();

    if(currentPlayer->isCheckMated()|| currentPlayer->getTime()  <= 0){
        onCheckMate();
    }

    onCleanup();

    return true;
}

void Chess::onCheckMate() {

    SDL_SetRenderDrawColor( renderer, 255, 255, 255, 255 );
    SDL_RenderClear( renderer );
    char * winner = const_cast<char *>((currentPlayer->getColor() == Piece::White) ? "Black Won" : "White Won");
    SDL_Color color{0,0,0};
    SDL_Surface * surface = TTF_RenderText_Solid(font, winner, color);
    SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_Rect dest;
    dest.x = 550;
    dest.y = 350;
    dest.w = surface->w;
    dest.h = surface->h;
    SDL_RenderCopy(renderer, texture, NULL, &dest);
    SDL_RenderPresent(renderer);
    SDL_UpdateWindowSurface(screen);


    sleep(10);
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}

void Chess::onCleanup() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(screen);
    TTF_CloseFont(this->font);
    TTF_Quit();
    SDL_Quit();
}

void Chess::onEvent(SDL_Event *event) {

    if(event->type == SDL_QUIT || currentPlayer->isCheckMated() || currentPlayer->getTime() - chrono::duration_cast<chrono::seconds>(chrono::steady_clock::now() - currentTime).count() <= 0) {
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
    img = pieceTextures[pieceDragging->getType()][pieceDragging->getColor()];
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
        if(attemptedMove.getIsPawnPromotion()){
            onPawnPromotion(attemptedMove);
        }

        updateTime();
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

void Chess::onPawnPromotion(Move & move) {

    char * arr[5] = {
            const_cast<char *>("Pawn Needs to Be promoted:"),
            const_cast<char *>("Type the number for the corrsponding pieces"),
            const_cast<char *>("(1) promote to Queen"),
            const_cast<char *>("(2) promote to Rook"),
            const_cast<char *>("(3) promote to Knight")
    };

    SDL_Rect dest;
    dest.x = 300;
    dest.y = 150;
    dest.w = 600;
    dest.h = 500;
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF);
    SDL_RenderFillRect(renderer,&dest);

    dest.h = 90;
    dest.w = 550;
    for(auto x: arr){
        SDL_Color color{255,255,255};
        SDL_Surface * surface = TTF_RenderText_Solid(font, x, color);
        SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_RenderCopy(renderer, texture, NULL, &dest);
        SDL_DestroyTexture(texture);
        SDL_FreeSurface(surface);

        dest.y+=90;
    }

    SDL_RenderPresent(renderer);
    SDL_UpdateWindowSurface(screen);

    int input = 0;
    SDL_Event Event;

    while(input == 0){
        while(SDL_PollEvent(&Event)){
            onPawnPromotionEvent(&Event,input);
        }
    }

    Piece::Color pawnColor = move.getMovingPiece()->getColor();

    if(input == 1){
        shared_ptr<Piece> newQueen = board.addPiece(Piece::Queen,pawnColor,move.getNewPosition());
        board.setPieceAt(move.getNewPosition(), newQueen);
        move.setPromotedToPiece(newQueen);
    } else if (input == 2){
        shared_ptr<Piece> newRook = board.addPiece(Piece::Rook,pawnColor,move.getNewPosition());
        board.setPieceAt(move.getNewPosition(), newRook);
        move.setPromotedToPiece(newRook);
    }else if (input == 3){
        shared_ptr<Piece> newKnight = board.addPiece(Piece::Knight,pawnColor,move.getNewPosition());
        board.setPieceAt(move.getNewPosition(), newKnight);
        move.setPromotedToPiece(newKnight);
    }

}

void Chess::onPawnPromotionEvent(SDL_Event *event, int & input) {
    if(!currentPlayer->isBot()){
        if(event->type == SDL_KEYDOWN){
            int temp = event->key.keysym.sym - 48;
            if(temp > 0 && temp <=3){
                input = temp;
            }
        }
    }else{
        input = (rand() % 3) + 1;
    }
}

void Chess::updateTime() {
    chrono::steady_clock::time_point end = chrono::steady_clock::now();
    float seconds = chrono::duration_cast<chrono::milliseconds>(end - currentTime).count() / 1000.00;
    currentPlayer->setTime(currentPlayer->getTime() - seconds);
    currentTime = chrono::steady_clock::now();
}



