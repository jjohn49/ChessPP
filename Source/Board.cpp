//
// Created by hugh on 12/9/23.
//

#include <utility>
#include "../Header/Board.h"
#include <SDL2/SDL_image.h>
#include <cmath>
#include <algorithm>

#define HEIGHT 800
#define WIDTH 800


Board::Board(std::vector<std::shared_ptr<Piece>> & captureWhitePieces, std::vector<std::shared_ptr<Piece>> & captureBlackPieces, std::vector<Move> & allGameMoves): captureWhitePieces(captureWhitePieces), captureBlackPieces(captureBlackPieces), allGameMoves(allGameMoves){
    screen = nullptr;
    running = true;
    renderer = nullptr;
    pieceDragging = nullptr;
}

void Board::printBoard() {
    for( auto & row: this->board){
        std::string rowStr = "";
        for(std::shared_ptr<Piece> & square: row){
            if(square != nullptr){
                rowStr += square->toString() + "\t";
            }else{
                rowStr+= "|\t\t\t\t|";
            }
        }
        std::cout<<rowStr<<"\n";
    }
}

void Board::movePiece(Move move) {
    std::pair<int,int> oldPos{move.getOldPosition()};
    std::pair<int,int> newPos{move.getNewPosition()};
    std::shared_ptr<Piece> piece = pieceDragging;

    //checvks if there was a piece on the square and adds it to the correct capturePieces vector
    if(this->getPieceAt(move.getNewPosition()) != nullptr){
        if(this->getPieceAt(move.getNewPosition())->getColor() == Piece::Black){
            this->captureBlackPieces.push_back(this->getPieceAt(move.getNewPosition()));
        }else{
            this->captureWhitePieces.push_back(this->getPieceAt(move.getNewPosition()));
        }
    }
    this->board[oldPos.first][oldPos.second] = nullptr;
    this->board[newPos.first][newPos.second] = piece;
    piece->setNewPosition(newPos);

    allGameMoves.push_back(move);
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
        //OnLoop();
        //OnRender();
    }

    OnCleanup();

    return 0;
}

std::shared_ptr<Piece> Board::getPieceAt(std::pair<int, int> pos) {
    return this->board[pos.first][pos.second];
}

std::vector<Move> Board::getAllMoves() {
    std::vector<Move> moves{};

    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            if(this->getPieceAt(std::make_pair(i,j)) != nullptr){
                std::shared_ptr<Piece> piece = this->getPieceAt(std::make_pair(i,j));
                std::vector<Move> pieceMoves = piece->getMoves(this->board);
                moves.insert(moves.end(), pieceMoves.begin(), pieceMoves.end());
            }
        }
    }

    if(pieceDragging != nullptr){
        std::vector<Move> draggingMoves{pieceDragging->getMoves(board)};
        moves.insert(moves.end(), draggingMoves.begin(), draggingMoves.end());
    }

    return moves;
}

bool Board::OnInit() {
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

void Board::OnEvent(SDL_Event *Event) {
    
    if(Event->type == SDL_MOUSEBUTTONDOWN) {
        std::copy(&board[0][0], &board[0][0] + 64, &copyBoard[0][0]);
        setPieceClicked(Event);
        //std::cout<< pieceDragging->toString()<<std::endl;
    }else if(Event->type ==SDL_MOUSEBUTTONUP){
        placePiece(Event, copyBoard);
    }
    else if(Event->type == SDL_QUIT) {
        running = false;
    }
}

void Board::setPieceClicked(SDL_Event *event) {
    int x, y;
    SDL_GetMouseState(&y, &x);
    std::pair<int,int> pos = std::make_pair(floor(x/100), floor(y/100));
    pieceDragging = getPieceAt(pos);
    board[pos.first][pos.second] = nullptr;
}

void Board::placePiece(SDL_Event *event, std::shared_ptr<Piece> copyBoard[8][8]) {
    int x, y;
    SDL_GetMouseState(&y, &x);
    std::pair<int,int> newPos = std::make_pair(floor(x/100), floor(y/100));
    Move attemptedMove{pieceDragging->getPosition(), newPos};
    std::vector<Move> moves = getMovesFor(pieceDragging->getColor());
    if(contains(moves, attemptedMove)){
        movePiece(attemptedMove);
    }else{
        std::copy(&copyBoard[0][0], &copyBoard[0][0] + 64, &board[0][0]);
    }
    pieceDragging = nullptr;
}

void Board::OnLoop() {

}

void Board::OnRender() {

}

void Board::OnCleanup() {
    SDL_Quit();
}

void Board::drawBoard()  {
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

             rect.w = darea.w / 8;
             rect.h = darea.h / 8;
             rect.x = column * rect.w;
             rect.y = row * rect.h;

             SDL_RenderFillRect(renderer, &rect);

             std::shared_ptr<Piece> piece = getPieceAt(std::make_pair(row,column));
             if(piece != nullptr){
                 img = IMG_LoadTexture(renderer, piece->getImagePath().c_str());
                 SDL_QueryTexture(img, NULL, NULL, &w, &h);
                 SDL_RenderCopy(renderer, img,NULL,&rect);
             }
         }

     }

    if(pieceDragging != nullptr){
        int x,y;
        SDL_GetMouseState(&x,&y);
        img = IMG_LoadTexture(renderer, pieceDragging->getImagePath().c_str());
        SDL_QueryTexture(img, NULL, NULL, &w, &h);
        rect.x = x - 50;
        rect.y = y - 50;
        SDL_RenderCopy(renderer, img,NULL,&rect);

        //SDL_UpdateWindowSurface(screen);

    }
    SDL_RenderPresent(renderer);
    SDL_UpdateWindowSurface(screen);
}

bool Board::isCheckFor(Piece::Color color) {
    std::vector<Move> allMoves{this->getAllMoves()};

    std::shared_ptr<King> targetKing = (color == Piece::White) ? std::make_shared<King>(whiteKing) : std::make_shared<King>(blackKing);

    for(Move &move: allMoves){
        if(move.getNewPosition() == targetKing->getPosition()){
            //free(targetKing);
            return true;
        }
    }

    //free(targetKing);
    return false;
}

//Gets all possible moves, makes a copy of the board before any move
//Then modifies board with each move, checking if they are still in check
//resets to original board after each move
std::vector<Move> Board::getMovesFor(Piece::Color color) {
    std::vector<Move> allMoves{getAllMoves()};
    std::vector<Move> movesForColor{};
    for(Move & move: allMoves){
        std::shared_ptr<Piece> p;
        if(getPieceAt(move.getOldPosition()) == nullptr){
            p = pieceDragging;
        }else{
            p = getPieceAt(move.getOldPosition());
        }
        if(p->getColor() == color){
            std::shared_ptr<Piece> copy[8][8];
            std::vector<Move> copyAllGameMoves = allGameMoves;
            std::copy(&board[0][0], &board[0][0]+64, &copy[0][0]);
            movePiece(move);
            if(!isCheckFor(color)){
                movesForColor.push_back(move);
            }
            std::copy(&copy[0][0], &copy[0][0]+64, &board[0][0]);
            allGameMoves = copyAllGameMoves;
        }
    }

    return movesForColor;
}

void Board::dragPiece() {
    
}

bool Board::contains(std::vector<Move> list, Move val) {
    for(Move & move: list){
        if(val == move){
            return true;
        }
    }

    return false;
}






