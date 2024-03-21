//
// Created by hugh on 12/9/23.
//

#include <utility>
#include "../Header/Board.h"
#include <cmath>
#include <algorithm>

//OS Specific Imports
#ifdef __linux__
    #include <SDL2/SDL_image.h>
#elif __APPLE__
    #include <SDL_image.h>
#endif



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
    //std::shared_ptr<Piece> piece = pieceDragging;
    std::shared_ptr<Piece> p = (this->getPieceAt(move.getOldPosition()) != nullptr)? getPieceAt(move.getOldPosition()) : pieceDragging;

    //checvks if there was a piece on the square and adds it to the correct capturePieces vector
    if(this->getPieceAt(move.getNewPosition()) != nullptr){
        if(this->getPieceAt(move.getNewPosition())->getColor() == Piece::Black){
            this->captureBlackPieces.push_back(this->getPieceAt(move.getNewPosition()));
        }else{
            this->captureWhitePieces.push_back(this->getPieceAt(move.getNewPosition()));
        }
    }

    if(move.getIsEnPessant()){
        if(p->getColor()==Piece::White) {
            this->captureBlackPieces.push_back(this->getPieceAt(std::make_pair(newPos.first - 1, newPos.second)));
            this->board[newPos.first-1][newPos.second] = nullptr;
        }else{
            this->captureWhitePieces.push_back(this->getPieceAt(std::make_pair(newPos.first + 1, newPos.second)));
            this->board[newPos.first+1][newPos.second] = nullptr;
        }
    }

    this->board[oldPos.first][oldPos.second] = nullptr;
    this->board[newPos.first][newPos.second] = p;
    p->setNewPosition(newPos);

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
                if(piece->getType() == Piece::Pawn){
                    std::optional<Move> enPessant{piece->getEnPessant(board, allGameMoves)};
                    if(enPessant){
                        moves.push_back(enPessant.value());
                    }
                }
            }
        }
    }

    if(pieceDragging != nullptr){
        std::vector<Move> draggingMoves{pieceDragging->getMoves(board)};
        if(pieceDragging->getType() == Piece::Pawn){
            std::optional<Move> enPessant{pieceDragging->getEnPessant(board, allGameMoves)};
            if(enPessant){
                draggingMoves.push_back(enPessant.value());
            }
        }
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
        placePiece(Event);
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

void Board::placePiece(SDL_Event *event) {
    int x, y;
    SDL_GetMouseState(&y, &x);
    std::pair<int,int> newPos = std::make_pair(floor(x/100), floor(y/100));
    Move attemptedMove{pieceDragging->getPosition(), newPos};
    std::vector<Move> moves = getMovesFor(pieceDragging->getColor());

    if(contains(moves, attemptedMove)){
        //need to find the exact move because of enpessant
        for(Move & move: moves){
            if(attemptedMove == move){
                movePiece(move);
            }
        }
    }else{
        std::copy(&copyBoard[0][0], &copyBoard[0][0] + 64, &board[0][0]);
    }
    pieceDragging = nullptr;
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
        std::shared_ptr<Piece> p = (this->getPieceAt(move.getOldPosition()) != nullptr)? getPieceAt(move.getOldPosition()) : pieceDragging;
        std::shared_ptr<Piece> copy[8][8];
        std::vector<Move> copyAllGameMoves = allGameMoves;
        std::copy(&board[0][0], &board[0][0]+64, &copy[0][0]);
        std::pair<int,int> oldPosition{p->getPosition()};
        movePiece(move);
        if(!isCheckFor(color)){
            movesForColor.push_back(move);
        }
        p->setNewPosition(oldPosition);
        std::copy(&copy[0][0], &copy[0][0]+64, &board[0][0]);

        allGameMoves = copyAllGameMoves;

    }

    return movesForColor;
}

bool Board::contains(std::vector<Move> list, Move val) {
    for(Move & move: list){
        if(val == move){
            return true;
        }
    }
    return false;
}

bool Board::contains(std::vector<Move> list, std::pair<int, int> position) {
    for(Move & move: list){
        if(move.getNewPosition() == position){
            return true;
        }
    }
    return false;
}

std::vector<Move> Board::getCastlingMoves(Piece::Color color) {
    std::vector<Move> moves{};

    int x = (color == Piece::White)? 0 : 7;
    std::shared_ptr<King> targetKing = (color == Piece::White)? std::make_shared<King>(whiteKing) : std::make_shared<King>(blackKing);
    std::shared_ptr<Rook> targetRookKS = (color == Piece::White)? std::make_shared<Rook>(whiteRooks[0]) : std::make_shared<Rook>(blackRooks[0]);
    std::shared_ptr<Rook> targetRookQS = (color == Piece::White)? std::make_shared<Rook>(whiteRooks[1]) : std::make_shared<Rook>(blackRooks[1]);

    std::vector<Move> opponentMoves{getMovesFor((color == Piece::White)? Piece::Black: Piece::White)};


    //checking if all necessary pieces have moved yet
    if(!(targetKing->moved() && targetRookKS->moved() && targetRookQS->moved())){
        //now checking if there is anything between them or if they ever go into check
        //QS
        for(int y = 3; y >= 0; y --){
            if(board[x][y] != nullptr || ( y >= 2 && contains(opponentMoves, std::make_pair(x,y)))){
                break;
            }

            if(y == 0){
                Move qsCastling{targetKing->getPosition(), std::make_pair(x,2)};
                qsCastling.setCastling("QS");
                moves.push_back(qsCastling);
            }
        }

        //KS
        for(int y = 5; y <= 7; y++){
            if(board[x][y] != nullptr || ( y <= 6 && contains(opponentMoves, std::make_pair(x,y)))){
                break;
            }

            if(y == 7){
                Move ksCastling{targetKing->getPosition(), std::make_pair(x,6)};
                ksCastling.setCastling("KS");
                moves.push_back(ksCastling);
            }
        }
    }

    return moves;
}








