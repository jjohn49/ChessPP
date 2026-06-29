//
// Created by hugh on 3/21/24.
//

#include "../Headers/Chess.h"
#include "../Headers/RoomCode.h"
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
    this->currentPlayer = whitePlayer;
    this->timeControl = NoTimer;
    this->useClock = false;
    this->botThinking = false;
    this->boardFlipped = false;
    this->isNetworkGame = false;
    this->myColor = Piece::NoColor;
    this->reconnecting = false;
    this->remoteDragging = false;
    this->remoteOrigin = {-1, -1};
    this->remotePieceType = Piece::NoType;
    this->remotePieceColor = Piece::NoColor;
    this->remoteCursorRowF = 0.0f;
    this->remoteCursorColF = 0.0f;
}

Chess::Chess(bool useBot, BotDifficulty level, Piece::Color botColor, TimeControl tc){
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
    this->timeControl = tc;
    this->useClock = (tc != NoTimer);
    this->botThinking = false;
    this->boardFlipped = (botColor == Piece::White);
    this->isNetworkGame = false;
    this->myColor = Piece::NoColor;
    this->reconnecting = false;
    this->remoteDragging = false;
    this->remoteOrigin = {-1, -1};
    this->remotePieceType = Piece::NoType;
    this->remotePieceColor = Piece::NoColor;
    this->remoteCursorRowF = 0.0f;
    this->remoteCursorColF = 0.0f;
    if(useClock){
        whitePlayer->setTime(static_cast<float>(tc));
        blackPlayer->setTime(static_cast<float>(tc));
        currentTime = chrono::steady_clock::now();
    }
}

void Chess::play() {
    onExecute();
}

void Chess::drawTime() {
    auto formatTime = [](float seconds) -> string {
        if(seconds < 0.0f) seconds = 0.0f;
        int totalSec = static_cast<int>(seconds);
        int mins = totalSec / 60;
        int secs = totalSec % 60;
        char buf[16];
        if(seconds < 10.0f) {
            int tenths = static_cast<int>((seconds - totalSec) * 10);
            snprintf(buf, sizeof(buf), "%d:%02d.%d", mins, secs, tenths);
        } else {
            snprintf(buf, sizeof(buf), "%d:%02d", mins, secs);
        }
        return string(buf);
    };

    // White clock: left side normally, right side when flipped
    // Black clock: right side normally, left side when flipped
    int whiteX = boardFlipped ? 1000 : 0;
    int blackX = boardFlipped ? 0 : 1000;

    SDL_Rect dest;
    dest.x = whiteX;
    dest.y = 350;
    dest.w = 200;
    dest.h = 100;

    if(currentPlayer->getColor() == Piece::White)
        SDL_SetRenderDrawColor(renderer, 0, 80, 0, 0xFF);
    else
        SDL_SetRenderDrawColor(renderer, 40, 40, 40, 0xFF);
    SDL_RenderFillRect(renderer, &dest);

    SDL_Color color{255, 255, 255};
    string whiteStr = formatTime(whitePlayer->getTime());
    SDL_Surface * surface = TTF_RenderText_Solid(font, whiteStr.c_str(), color);
    SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_RenderCopy(renderer, texture, NULL, &dest);
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);

    // Black clock
    dest.x = blackX;
    dest.y = 350;
    dest.w = 200;
    dest.h = 100;

    if(currentPlayer->getColor() == Piece::Black)
        SDL_SetRenderDrawColor(renderer, 0, 80, 0, 0xFF);
    else
        SDL_SetRenderDrawColor(renderer, 40, 40, 40, 0xFF);
    SDL_RenderFillRect(renderer, &dest);

    string blackStr = formatTime(blackPlayer->getTime());
    surface = TTF_RenderText_Solid(font, blackStr.c_str(), color);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_RenderCopy(renderer, texture, NULL, &dest);
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}

void Chess::drawBoard() {
    SDL_Rect rect, darea;
    SDL_Texture * img = nullptr;
    int w,h;

    /* Get the Size of drawing surface */
    SDL_RenderGetViewport(renderer, &darea);

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
            else if((row + column) % 2 == 0){
                SDL_SetRenderDrawColor(renderer, 1, 55, 32, 0xFF);
            }else {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0xFF);
            }

            int drawCol = boardFlipped ? (7 - column) : column;
            int drawRow = boardFlipped ? row : (7 - row);
            rect.w = 100;
            rect.h = 100;
            rect.x = drawCol * 100 + 200;
            rect.y = drawRow * 100;

            SDL_RenderFillRect(renderer, &rect);

            std::shared_ptr piece = board.getPieceAt(row,column);
            // Hide the opponent's lifted piece at its origin while they drag it
            // (the piece stays in the grid so an incoming MOVE can still find it).
            bool hideForRemoteDrag = remoteDragging && row == remoteOrigin.first && column == remoteOrigin.second;
            if(piece != nullptr && !hideForRemoteDrag){
                img = pieceTextures[piece->getType()][piece->getColor()];
                SDL_QueryTexture(img, NULL, NULL, &w, &h);
                SDL_RenderCopy(renderer, img,NULL,&rect);
            }
        }
    }

    // Draw the piece being dragged at the cursor position
    if(pieceDragging != nullptr){
        int mx, my;
        SDL_GetMouseState(&mx, &my);
        img = pieceTextures[pieceDragging->getType()][pieceDragging->getColor()];
        rect.w = 100;
        rect.h = 100;
        rect.x = mx - 50;
        rect.y = my - 50;
        SDL_RenderCopy(renderer, img, NULL, &rect);
    }

    // Draw the opponent's in-flight dragged piece (live mirroring)
    drawRemoteGhost();
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
        return false;
    }

    showMenu();
    if(!running) {
        onCleanup();
        return true;
    }

    SDL_Event Event;
    auto lastPing = chrono::steady_clock::now();

    while(this->running) {
        if(useClock) {
            updateTime();
            if(currentPlayer->getTime() <= 0.0f) {
                currentPlayer->setTime(0.0f);
                onCheckMate();
                break;
            }
        }

        while(SDL_PollEvent(&Event)) {
            onEvent(&Event);
        }

        if(isNetworkGame) {
            pollNetwork();
            if(!running) break;
        }

        // While dragging, mirror the cursor to the opponent (canonical board space).
        if(isNetworkGame && net && pieceDragging) {
            int mx, my;
            SDL_GetMouseState(&mx, &my);
            double duCol = (mx - 200) / 100.0;
            double duRow = my / 100.0;
            double fcol = boardFlipped ? (8.0 - duCol) : duCol;
            double frow = boardFlipped ? duRow : (8.0 - duRow);
            net->send(NetProtocol::serializeDrag(frow, fcol));
        }

        // Heartbeat: periodic traffic so a broken connection is detected promptly
        // (a failed write flips the session to disconnected -> triggers reconnect).
        if(isNetworkGame && net) {
            auto now = chrono::steady_clock::now();
            if(chrono::duration_cast<chrono::milliseconds>(now - lastPing).count() > 1000) {
                long long ms = chrono::duration_cast<chrono::milliseconds>(now.time_since_epoch()).count();
                net->send(NetProtocol::serializePing(ms));
                lastPing = now;
            }
        }

        // Launch bot search on a background thread
        if(running && currentPlayer->isBot() && !botThinking) {
            botThinking = true;
            auto botPlayer = currentPlayer;
            botFuture = std::async(std::launch::async, [botPlayer]() {
                return botPlayer->move();
            });
        }

        // Check if the bot has finished thinking
        if(botThinking && botFuture.valid() &&
           botFuture.wait_for(chrono::milliseconds(0)) == std::future_status::ready) {
            bool moveOk = botFuture.get();
            botThinking = false;
            if(!moveOk) {
                running = false;
                currentPlayer->setTime(-100);
            } else {
                board.print();
                if(useClock) updateTime();
                if(useClock) currentTime = chrono::steady_clock::now();
                currentPlayer = (currentPlayer->getColor()==Piece::White)? blackPlayer:whitePlayer;
                if(currentPlayer->isCheckMated()){
                    onCheckMate();
                } else if(currentPlayer->isStalemated()){
                    onStalemate();
                }
            }
        }

        drawBoard();
        if(useClock) {
            drawTime();
        }
        SDL_RenderPresent(renderer);
        SDL_UpdateWindowSurface(screen);

        SDL_Delay(16);
    }

    onCleanup();

    return true;
}

void Chess::onCheckMate() {

    running = false;
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

void Chess::onStalemate() {
    running = false;
    SDL_SetRenderDrawColor( renderer, 255, 255, 255, 255 );
    SDL_RenderClear( renderer );
    SDL_Color color{0,0,0};
    SDL_Surface * surface = TTF_RenderText_Solid(font, "Stalemate - Draw!", color);
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

    if(event->type == SDL_QUIT) {
        running = false;
        return;
    }

    bool inputAllowed = !currentPlayer->isBot() && (!isNetworkGame || currentPlayer->getColor() == myColor);
    if(inputAllowed){
        if(event->type == SDL_MOUSEBUTTONDOWN) {
            if(pieceDragging == nullptr){
                setPieceDragging(event);
            }else{
                onPlacePieceDragging(event);
                // Check game-over after the move switched currentPlayer
                if(currentPlayer->isCheckMated()){
                    onCheckMate();
                } else if(currentPlayer->isStalemated()){
                    onStalemate();
                }
            }

        }else if(event->type == SDL_MOUSEMOTION){
            if(pieceDragging != nullptr){
                onPieceDraggingMoved(event);
            }
        }
    }
    // Bot moves are handled in onExecute() main loop
}

void Chess::setPieceDragging(SDL_Event *event) {
    int x, y;
    SDL_GetMouseState(&y, &x);
    int row = convertYAxisToRow(x);
    int col = static_cast<int>(floor((y-200)/100));
    if(boardFlipped) {
        row = 7 - row;
        col = 7 - col;
    }
    std::pair<int,int> pos = std::make_pair(row, col);

    std::shared_ptr<Piece> pieceChosen = board.getPieceAt(pos);
    if(pieceChosen != nullptr && pieceChosen->getColor() == currentPlayer->getColor()){
        pieceDragging = pieceChosen;
        board.setPieceAt(pos, nullptr);
        if(isNetworkGame && net) net->send(NetProtocol::serializePickup(pos.first, pos.second));
    }
}

void Chess::onPieceDraggingMoved(SDL_Event * event) {
    // Dragged piece is now rendered in drawBoard() every frame.
    // This handler kept so the event dispatch doesn't need changing.
}

void Chess::onPlacePieceDragging(SDL_Event *event) {
    int x, y;
    SDL_GetMouseState(&y,&x);
    int row = convertYAxisToRow(x);
    int col = static_cast<int>(floor((y-200)/100));
    if(boardFlipped) {
        row = 7 - row;
        col = 7 - col;
    }
    std::pair<int,int> pos = std::make_pair(row, col);

    Move attemptedMove = Move(pieceDragging->getPosition(), pos, pieceDragging, board.getPieceAt(pos));

    if(currentPlayer->movePiece(attemptedMove)){
        if(attemptedMove.getIsPawnPromotion()){
            onPawnPromotion(attemptedMove);
        }

        if(isNetworkGame) sendMoveMessage(attemptedMove);
        if(useClock) currentTime = chrono::steady_clock::now();
        currentPlayer = (currentPlayer->getColor()==Piece::White)? blackPlayer:whitePlayer;
    } else {
        if(isNetworkGame && net) net->send(NetProtocol::serializeDrop());
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
    const char * labels[3] = {"Queen", "Rook", "Knight"};
    const int btnW = 160;
    const int btnH = 50;
    const int popupW = 3 * btnW + 40;  // 3 buttons + padding
    const int popupH = 120;
    const int popupX = (WIDTH - popupW) / 2;
    const int popupY = (HEIGHT - popupH) / 2;
    const int btnY = popupY + 55;
    const int btnSpacing = 10;
    const int firstBtnX = popupX + 15;

    SDL_Rect btnRects[3];
    for (int i = 0; i < 3; i++) {
        btnRects[i] = {firstBtnX + i * (btnW + btnSpacing), btnY, btnW, btnH};
    }

    int hoveredBtn = -1;
    int input = 0;
    SDL_Event Event;

    while (input == 0) {
        // Draw the board underneath
        drawBoard();

        // Draw popup background
        SDL_Rect popup = {popupX, popupY, popupW, popupH};
        SDL_SetRenderDrawColor(renderer, 40, 40, 40, 0xFF);
        SDL_RenderFillRect(renderer, &popup);

        // Draw title text
        SDL_Color white{255, 255, 255};
        SDL_Surface * surface = TTF_RenderText_Solid(font, "Promote Pawn", white);
        SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_Rect titleDest;
        titleDest.x = popupX + (popupW - surface->w) / 2;
        titleDest.y = popupY + 10;
        titleDest.w = surface->w;
        titleDest.h = surface->h;
        SDL_RenderCopy(renderer, texture, NULL, &titleDest);
        SDL_DestroyTexture(texture);
        SDL_FreeSurface(surface);

        // Draw buttons
        for (int i = 0; i < 3; i++) {
            if (i == hoveredBtn)
                SDL_SetRenderDrawColor(renderer, 90, 90, 90, 0xFF);
            else
                SDL_SetRenderDrawColor(renderer, 60, 60, 60, 0xFF);
            SDL_RenderFillRect(renderer, &btnRects[i]);

            surface = TTF_RenderText_Solid(font, labels[i], white);
            texture = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_Rect textDest;
            textDest.x = btnRects[i].x + (btnW - surface->w) / 2;
            textDest.y = btnRects[i].y + (btnH - surface->h) / 2;
            textDest.w = surface->w;
            textDest.h = surface->h;
            SDL_RenderCopy(renderer, texture, NULL, &textDest);
            SDL_DestroyTexture(texture);
            SDL_FreeSurface(surface);
        }

        SDL_RenderPresent(renderer);
        SDL_UpdateWindowSurface(screen);

        while (SDL_PollEvent(&Event)) {
            if (Event.type == SDL_QUIT) { running = false; return; }
            onPawnPromotionEvent(&Event, input);
            if (Event.type == SDL_MOUSEBUTTONDOWN && Event.button.button == SDL_BUTTON_LEFT) {
                int mx = Event.button.x;
                int my = Event.button.y;
                for (int i = 0; i < 3; i++) {
                    if (mx >= btnRects[i].x && mx <= btnRects[i].x + btnW &&
                        my >= btnRects[i].y && my <= btnRects[i].y + btnH) {
                        input = i + 1;
                        break;
                    }
                }
            }
            if (Event.type == SDL_MOUSEMOTION) {
                int mx = Event.motion.x;
                int my = Event.motion.y;
                hoveredBtn = -1;
                for (int i = 0; i < 3; i++) {
                    if (mx >= btnRects[i].x && mx <= btnRects[i].x + btnW &&
                        my >= btnRects[i].y && my <= btnRects[i].y + btnH) {
                        hoveredBtn = i;
                        break;
                    }
                }
            }
        }
        SDL_Delay(16);
    }

    Piece::Type promoteTypes[] = {Piece::Queen, Piece::Rook, Piece::Knight};
    applyPromotion(move, promoteTypes[input - 1]);
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

void Chess::drawMenuScreen(const vector<string> & lines, const string & title, int hoveredButton) {
    SDL_SetRenderDrawColor(renderer, 30, 30, 30, 0xFF);
    SDL_RenderClear(renderer);

    SDL_Color titleColor{255, 200, 50};
    SDL_Color textColor{255, 255, 255};

    // Draw title
    SDL_Surface * surface = TTF_RenderText_Solid(font, title.c_str(), titleColor);
    SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect dest;
    dest.x = (WIDTH - surface->w) / 2;
    dest.y = 150;
    dest.w = surface->w;
    dest.h = surface->h;
    SDL_RenderCopy(renderer, texture, NULL, &dest);
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);

    // Draw buttons
    const int btnW = 400;
    const int btnH = 45;
    const int btnX = (WIDTH - btnW) / 2;
    int btnY = 260;

    for (int i = 0; i < static_cast<int>(lines.size()); i++) {
        SDL_Rect btnRect;
        btnRect.x = btnX;
        btnRect.y = btnY;
        btnRect.w = btnW;
        btnRect.h = btnH;

        if (i == hoveredButton) {
            SDL_SetRenderDrawColor(renderer, 90, 90, 90, 0xFF);
        } else {
            SDL_SetRenderDrawColor(renderer, 60, 60, 60, 0xFF);
        }
        SDL_RenderFillRect(renderer, &btnRect);

        surface = TTF_RenderText_Solid(font, lines[i].c_str(), textColor);
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        dest.x = btnX + (btnW - surface->w) / 2;
        dest.y = btnY + (btnH - surface->h) / 2;
        dest.w = surface->w;
        dest.h = surface->h;
        SDL_RenderCopy(renderer, texture, NULL, &dest);
        SDL_DestroyTexture(texture);
        SDL_FreeSurface(surface);

        btnY += 55;
    }

    SDL_RenderPresent(renderer);
    SDL_UpdateWindowSurface(screen);
}

int Chess::menuSelectFromScreen(const vector<string> & labels, const string & title, int optionCount) {
    const int btnW = 400;
    const int btnH = 45;
    const int btnX = (WIDTH - btnW) / 2;
    const int startY = 260;
    const int spacing = 55;

    int hoveredButton = -1;
    drawMenuScreen(labels, title, hoveredButton);

    SDL_Event event;
    while (true) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
                return 0;
            }
            if (event.type == SDL_KEYDOWN) {
                int key = event.key.keysym.sym - '0';
                if (key >= 1 && key <= optionCount) return key;
            }
            if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                int mx = event.button.x;
                int my = event.button.y;
                for (int i = 0; i < optionCount; i++) {
                    int by = startY + i * spacing;
                    if (mx >= btnX && mx <= btnX + btnW && my >= by && my <= by + btnH) {
                        return i + 1;
                    }
                }
            }
            if (event.type == SDL_MOUSEMOTION) {
                int mx = event.motion.x;
                int my = event.motion.y;
                int newHover = -1;
                for (int i = 0; i < optionCount; i++) {
                    int by = startY + i * spacing;
                    if (mx >= btnX && mx <= btnX + btnW && my >= by && my <= by + btnH) {
                        newHover = i;
                        break;
                    }
                }
                if (newHover != hoveredButton) {
                    hoveredButton = newHover;
                    drawMenuScreen(labels, title, hoveredButton);
                }
            }
        }
        SDL_Delay(16);
    }
}

void Chess::showMenu() {
    // Screen 1: Game Mode
    bool useBot = false;
    {
        vector<string> labels = {"Play vs Human (Local)", "Play vs Human (Online)", "Play vs Bot"};
        int input = menuSelectFromScreen(labels, "Select Game Mode", 3);
        if (!running) return;
        if (input == 2) { setupNetworkGame(); return; }
        useBot = (input == 3);
    }

    // Screen 2 & 3: Bot settings (only if bot chosen)
    BotDifficulty difficulty = Easy;
    Piece::Color botColor = Piece::Black;
    if (useBot) {
        // Difficulty
        {
            vector<string> labels = {
                "Random", "Super Easy", "Easy",
                "Intermediate", "Hard", "Expert"
            };
            int input = menuSelectFromScreen(labels, "Select Bot Difficulty", 6);
            if (!running) return;
            BotDifficulty difficulties[] = {Random, SuperEasy, Easy, Intermediate, Hard, Expert};
            difficulty = difficulties[input - 1];
        }

        // Player Color
        {
            vector<string> labels = {"Play as White", "Play as Black"};
            int input = menuSelectFromScreen(labels, "Select Your Color", 2);
            if (!running) return;
            botColor = (input == 1) ? Piece::Black : Piece::White;
        }
    }

    // Screen 4: Time Control
    TimeControl tc = NoTimer;
    {
        vector<string> labels = {
            "No Timer", "Bullet (1 min)", "Blitz (3 min)",
            "5 Minutes", "Rapid (10 min)", "20 Minutes",
            "Classical (30 min)"
        };
        int input = menuSelectFromScreen(labels, "Select Time Control", 7);
        if (!running) return;
        TimeControl controls[] = {NoTimer, Bullet, Blitz, FiveMin, Rapid, TwentyMin, Classical};
        tc = controls[input - 1];
    }

    // Configure game based on selections
    if (useBot) {
        whitePlayer = (botColor == Piece::White)
            ? make_shared<Bot>(Bot(difficulty, botColor, &board))
            : make_shared<Player>(Player(Piece::White, &board));
        blackPlayer = (botColor == Piece::Black)
            ? make_shared<Bot>(Bot(difficulty, botColor, &board))
            : make_shared<Player>(Player(Piece::Black, &board));
    } else {
        whitePlayer = make_shared<Player>(Player(Piece::White, &board));
        blackPlayer = make_shared<Player>(Player(Piece::Black, &board));
    }
    currentPlayer = whitePlayer;
    boardFlipped = (useBot && botColor == Piece::White);

    timeControl = tc;
    useClock = (tc != NoTimer);
    if (useClock) {
        whitePlayer->setTime(static_cast<float>(tc));
        blackPlayer->setTime(static_cast<float>(tc));
        currentTime = chrono::steady_clock::now();
    }
}

void Chess::updateTime() {
    if(!useClock) return;
    auto now = chrono::steady_clock::now();
    float elapsed = chrono::duration<float>(now - currentTime).count();
    currentPlayer->setTime(currentPlayer->getTime() - elapsed);
    currentTime = now;
}

// ===================== LAN Multiplayer =====================

char Chess::promoTypeToChar(Piece::Type t) {
    switch(t) {
        case Piece::Queen:  return 'Q';
        case Piece::Rook:   return 'R';
        case Piece::Knight: return 'N';
        default:            return '-';
    }
}

Piece::Type Chess::promoCharToType(char c) {
    switch(c) {
        case 'Q': return Piece::Queen;
        case 'R': return Piece::Rook;
        case 'N': return Piece::Knight;
        default:  return Piece::NoType;
    }
}

void Chess::applyPromotion(Move & move, Piece::Type type) {
    Piece::Color pawnColor = move.getMovingPiece()->getColor();
    shared_ptr<Piece> promoted = board.addPiece(type, pawnColor, move.getNewPosition());
    board.setPieceAt(move.getNewPosition(), promoted);
    move.setPromotedToPiece(promoted);
    board.recordPromotion(promoted);   // keep moveHistory authoritative for resync
}

void Chess::drawInfoScreen(const std::string & title, const std::vector<std::string> & lines) {
    SDL_SetRenderDrawColor(renderer, 30, 30, 30, 0xFF);
    SDL_RenderClear(renderer);

    SDL_Color titleColor{255, 200, 50};
    SDL_Color textColor{255, 255, 255};

    SDL_Surface * surface = TTF_RenderText_Solid(font, title.c_str(), titleColor);
    SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect dest;
    dest.x = (WIDTH - surface->w) / 2;
    dest.y = 150;
    dest.w = surface->w;
    dest.h = surface->h;
    SDL_RenderCopy(renderer, texture, NULL, &dest);
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);

    int y = 270;
    for(const std::string & line : lines) {
        if(!line.empty()) {
            surface = TTF_RenderText_Solid(font, line.c_str(), textColor);
            texture = SDL_CreateTextureFromSurface(renderer, surface);
            dest.x = (WIDTH - surface->w) / 2;
            dest.y = y;
            dest.w = surface->w;
            dest.h = surface->h;
            SDL_RenderCopy(renderer, texture, NULL, &dest);
            SDL_DestroyTexture(texture);
            SDL_FreeSurface(surface);
        }
        y += 45;
    }

    SDL_RenderPresent(renderer);
    SDL_UpdateWindowSurface(screen);
}

void Chess::setupNetworkGame() {
    int choice = menuSelectFromScreen({"Host Game", "Join Game"}, "Online Multiplayer", 2);
    if(!running) return;
    if(choice == 1) hostGameFlow();
    else joinGameFlow();
}

void Chess::hostGameFlow() {
    int colorChoice = menuSelectFromScreen({"Play as White", "Play as Black", "Random"}, "Choose Your Color", 3);
    if(!running) return;
    Piece::Color hostColor;
    if(colorChoice == 1) hostColor = Piece::White;
    else if(colorChoice == 2) hostColor = Piece::Black;
    else hostColor = (rand() % 2) ? Piece::White : Piece::Black;
    myColor = hostColor;

    std::vector<std::string> tcLabels = {
        "No Timer", "Bullet (1 min)", "Blitz (3 min)", "5 Minutes",
        "Rapid (10 min)", "20 Minutes", "Classical (30 min)"
    };
    int tcChoice = menuSelectFromScreen(tcLabels, "Select Time Control", 7);
    if(!running) return;
    TimeControl controls[] = {NoTimer, Bullet, Blitz, FiveMin, Rapid, TwentyMin, Classical};
    TimeControl tc = controls[tcChoice - 1];

    net = std::make_unique<NetSession>();
    if(!net->host(RoomCode::kLanPort)) {
        drawInfoScreen("Host Game", {"Could not start the server.",
            "Port " + std::to_string(RoomCode::kLanPort) + " may already be in use.",
            "", "Returning to menu..."});
        SDL_Delay(2500);
        running = false;
        return;
    }

    std::string code = RoomCode::encode(NetSession::getLocalLanIpRaw());
    if(!waitingScreen(code)) {
        running = false;
        return;
    }

    net->send(NetProtocol::serializeHello(NetProtocol::kProtocolVersion));
    char joinerColor = (myColor == Piece::White) ? 'B' : 'W';
    int tcSeconds = (tc == NoTimer) ? -1 : static_cast<int>(tc);
    net->send(NetProtocol::serializeAssign(joinerColor, tcSeconds));

    finishNetworkSetup(tc);
}

void Chess::joinGameFlow() {
    std::string input = textInputScreen("Join Game", "Enter room code (or IP:port):");
    if(!running) return;
    if(input.empty()) { running = false; return; }

    std::string ip;
    std::uint16_t port = RoomCode::kLanPort;
    auto colon = input.find(':');
    if(colon != std::string::npos) {
        ip = input.substr(0, colon);
        try {
            port = static_cast<std::uint16_t>(std::stoi(input.substr(colon + 1)));
        } catch(...) {
            ip.clear();
        }
    } else {
        auto decoded = RoomCode::decode(input);
        if(decoded) ip = RoomCode::ipv4ToString(*decoded);
    }
    if(ip.empty()) {
        drawInfoScreen("Join Game", {"Invalid room code.", "", "Returning to menu..."});
        SDL_Delay(2500);
        running = false;
        return;
    }

    drawInfoScreen("Join Game", {"Connecting to " + ip + " ..."});
    net = std::make_unique<NetSession>();
    if(!net->join(ip, port, 5000)) {
        drawInfoScreen("Join Game", {"Could not connect to " + ip + ".", "", "Returning to menu..."});
        SDL_Delay(2500);
        running = false;
        return;
    }

    net->send(NetProtocol::serializeHello(NetProtocol::kProtocolVersion));
    auto assign = awaitAssign(10000);
    if(!assign) {
        drawInfoScreen("Join Game", {"Did not receive game start.", "", "Returning to menu..."});
        SDL_Delay(2500);
        running = false;
        return;
    }
    myColor = (assign->colorChar == 'W') ? Piece::White : Piece::Black;
    TimeControl tc = (assign->tcSeconds < 0) ? NoTimer : static_cast<TimeControl>(assign->tcSeconds);
    finishNetworkSetup(tc);
}

void Chess::finishNetworkSetup(TimeControl tc) {
    isNetworkGame = true;
    whitePlayer = make_shared<Player>(Player(Piece::White, &board));
    blackPlayer = make_shared<Player>(Player(Piece::Black, &board));
    currentPlayer = whitePlayer;
    boardFlipped = (myColor == Piece::Black);
    timeControl = tc;
    useClock = (tc != NoTimer);
    if(useClock) {
        whitePlayer->setTime(static_cast<float>(tc));
        blackPlayer->setTime(static_cast<float>(tc));
        currentTime = chrono::steady_clock::now();
    }
}

bool Chess::waitingScreen(const std::string & code) {
    SDL_Event e;
    while(running) {
        while(SDL_PollEvent(&e)) {
            if(e.type == SDL_QUIT) { running = false; return false; }
            if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) return false;
        }
        drawInfoScreen("Host Game", {
            "Room Code:   " + code,
            "",
            "Your IP:  " + NetSession::getLocalLanIp(),
            "",
            "Waiting for opponent to join...",
            "(Press Esc to cancel)"
        });
        if(net->acceptOne(50)) return true;
        SDL_Delay(16);
    }
    return false;
}

std::string Chess::textInputScreen(const std::string & title, const std::string & prompt) {
    std::string buffer;
    std::string result;
    bool done = false;
    SDL_StartTextInput();
    SDL_Event e;
    while(running && !done) {
        while(SDL_PollEvent(&e)) {
            if(e.type == SDL_QUIT) { running = false; SDL_StopTextInput(); return ""; }
            if(e.type == SDL_TEXTINPUT) {
                buffer += e.text.text;
            } else if(e.type == SDL_KEYDOWN) {
                SDL_Keycode k = e.key.keysym.sym;
                if(k == SDLK_BACKSPACE && !buffer.empty()) buffer.pop_back();
                else if(k == SDLK_RETURN || k == SDLK_KP_ENTER) { result = buffer; done = true; }
                else if(k == SDLK_ESCAPE) { result = ""; done = true; }
            }
        }
        drawInfoScreen(title, {prompt, "", "> " + buffer + "_", "", "(Enter to connect, Esc to cancel)"});
        SDL_Delay(16);
    }
    SDL_StopTextInput();
    return result;
}

std::optional<NetProtocol::Assign> Chess::awaitAssign(int timeoutMs) {
    int elapsed = 0;
    SDL_Event e;
    while(running && elapsed < timeoutMs) {
        while(SDL_PollEvent(&e)) {
            if(e.type == SDL_QUIT) { running = false; return std::nullopt; }
        }
        while(auto line = net->poll()) {
            if(NetProtocol::messageType(*line) == NetProtocol::Type::Assign) {
                return NetProtocol::parseAssign(*line);
            }
        }
        if(!net->isConnected()) return std::nullopt;
        drawInfoScreen("Join Game", {"Connected!", "", "Waiting for game to start..."});
        SDL_Delay(16);
        elapsed += 16;
    }
    return std::nullopt;
}

void Chess::sendMoveMessage(Move & move) {
    if(!isNetworkGame || !net) return;
    NetProtocol::NetMove nm;
    nm.fromRow = move.getOldPosition().first;
    nm.fromCol = move.getOldPosition().second;
    nm.toRow = move.getNewPosition().first;
    nm.toCol = move.getNewPosition().second;
    nm.promo = '-';
    if(move.getIsPawnPromotion() && move.getPromotedToPiece() != nullptr) {
        nm.promo = promoTypeToChar(move.getPromotedToPiece()->getType());
    }
    net->send(NetProtocol::serializeMove(nm));
}

void Chess::applyRemoteMove(const NetProtocol::NetMove & nm) {
    std::pair<int,int> from{nm.fromRow, nm.fromCol};
    std::pair<int,int> to{nm.toRow, nm.toCol};

    shared_ptr<Piece> mover = board.getPieceAt(from);
    if(mover == nullptr) {
        std::cerr << "Network desync: no piece at source square" << std::endl;
        if(net && net->role() == NetSession::Role::Host) sendStateResync();
        return;
    }

    Move m(from, to, mover, board.getPieceAt(to));
    if(currentPlayer->movePiece(m)) {
        if(m.getIsPawnPromotion()) {
            applyPromotion(m, promoCharToType(nm.promo));
        }
        if(useClock) currentTime = chrono::steady_clock::now();
        currentPlayer = (currentPlayer->getColor() == Piece::White) ? blackPlayer : whitePlayer;
        remoteDragging = false;
        if(currentPlayer->isCheckMated()) onCheckMate();
        else if(currentPlayer->isStalemated()) onStalemate();
    } else {
        std::cerr << "Network desync: illegal remote move" << std::endl;
        if(net && net->role() == NetSession::Role::Host) sendStateResync();
    }
}

void Chess::handleNetLine(const std::string & line) {
    using namespace NetProtocol;
    switch(messageType(line)) {
        case Type::Move: {
            auto nm = parseMove(line);
            if(nm) applyRemoteMove(*nm);
            break;
        }
        case Type::Ping: {
            auto ms = parsePing(line);
            if(ms && net) net->send(serializePong(*ms));
            break;
        }
        case Type::Pickup: {
            auto sq = parsePickup(line);
            if(sq) {
                shared_ptr<Piece> p = board.getPieceAt({sq->row, sq->col});
                if(p) {
                    remoteDragging = true;
                    remoteOrigin = {sq->row, sq->col};
                    remotePieceType = p->getType();
                    remotePieceColor = p->getColor();
                    remoteCursorRowF = sq->row + 0.5f;
                    remoteCursorColF = sq->col + 0.5f;
                }
            }
            break;
        }
        case Type::Drag: {
            auto d = parseDrag(line);
            if(d && remoteDragging) {
                remoteCursorRowF = static_cast<float>(d->rowF);
                remoteCursorColF = static_cast<float>(d->colF);
            }
            break;
        }
        case Type::Drop:
            remoteDragging = false;
            break;
        case Type::State: {
            auto st = parseState(line);
            if(st) {
                board.loadFromMoves(st->moves);
                currentPlayer = (st->turn == 'W') ? whitePlayer : blackPlayer;
                if(useClock) {
                    whitePlayer->setTime(static_cast<float>(st->whiteTime));
                    blackPlayer->setTime(static_cast<float>(st->blackTime));
                    currentTime = chrono::steady_clock::now();
                }
                remoteDragging = false;
                pieceDragging.reset();
            }
            break;
        }
        case Type::Hello:
        case Type::Assign:
        case Type::Pong:
        case Type::Unknown:
        default:
            break;
    }
}

void Chess::pollNetwork() {
    if(!isNetworkGame || !net) return;
    while(auto line = net->poll()) {
        handleNetLine(*line);
        if(!running) return;
    }
    if(!net->isConnected()) {
        reconnectFlow();
    }
}

void Chess::sendStateResync() {
    if(!net) return;
    NetProtocol::State s;
    s.turn = (currentPlayer->getColor() == Piece::White) ? 'W' : 'B';
    s.whiteTime = whitePlayer->getTime();
    s.blackTime = blackPlayer->getTime();
    s.moves = board.getMoveHistoryAsNetMoves();
    net->send(NetProtocol::serializeState(s));
}

bool Chess::reconnectFlow() {
    if(!net) { running = false; return false; }
    reconnecting = true;
    remoteDragging = false;
    pieceDragging.reset();

    const int maxAttempts = 15;
    for(int i = 0; i < maxAttempts && running; i++) {
        SDL_Event e;
        while(SDL_PollEvent(&e)) {
            if(e.type == SDL_QUIT) { running = false; reconnecting = false; return false; }
        }
        drawInfoScreen("Reconnecting", {
            "Connection to the opponent was lost.",
            "",
            "Attempting to reconnect...  (" + std::to_string(i + 1) + "/" + std::to_string(maxAttempts) + ")"
        });
        if(net->reconnect(1000)) {
            reconnecting = false;
            // Host is authoritative: push the full game state to re-sync the joiner.
            if(net->role() == NetSession::Role::Host) sendStateResync();
            return true;
        }
    }

    drawInfoScreen("Connection Lost", {"Could not reconnect to the opponent.", "", "Returning to menu..."});
    SDL_Delay(2500);
    running = false;
    reconnecting = false;
    return false;
}

void Chess::drawRemoteGhost() {
    if(!remoteDragging) return;
    SDL_Texture * img = pieceTextures[remotePieceType][remotePieceColor];
    if(img == nullptr) return;

    // Inverse of the send mapping: canonical board-space -> this peer's pixels,
    // honoring our own boardFlipped. Centers the sprite on the opponent's cursor.
    double duCol = boardFlipped ? (8.0 - remoteCursorColF) : remoteCursorColF;
    double duRow = boardFlipped ? remoteCursorRowF : (8.0 - remoteCursorRowF);

    SDL_Rect rect;
    rect.w = 100;
    rect.h = 100;
    rect.x = static_cast<int>(duCol * 100 + 200) - 50;
    rect.y = static_cast<int>(duRow * 100) - 50;
    SDL_RenderCopy(renderer, img, NULL, &rect);
}



