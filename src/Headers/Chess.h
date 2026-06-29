//
// Created by hugh on 3/21/24.
//

#ifndef CHESSPP_CHESS_H
#define CHESSPP_CHESS_H

#include <iostream>
#include "Board.h"
#include "Player.h"
#include "Bot.h"
#include <unordered_map>
#include <chrono>
#include <future>
#include <memory>
#include <optional>
#include <vector>
#include "NetSession.h"
#include "NetProtocol.h"
#ifdef __linux__
    #include <SDL2/SDL.h>
    #include <SDL2/SDL_ttf.h>
#elif __APPLE__

    #include <SDL.h>
    #include <SDL_ttf.h>
    
#endif


enum TimeControl {
	Bullet = 60,
	Blitz = 180,
	FiveMin = 300,
	Rapid = 600,
	TwentyMin = 1200,
	Classical = 1800,
	NoTimer = -1
};

class Chess {
public:
    Board board;
protected:

    bool running;
    SDL_Window * screen;
    SDL_Renderer * renderer;
    TTF_Font * font;
    std::shared_ptr<Piece> pieceDragging;
    shared_ptr<Player> currentPlayer;
    shared_ptr<Player>  whitePlayer;
    shared_ptr<Player>  blackPlayer;
    unordered_map<Piece::Type, unordered_map<Piece::Color,SDL_Texture *>> pieceTextures;
    chrono::steady_clock::time_point currentTime;
    TimeControl timeControl;
    bool useClock;
    bool botThinking;
    std::future<bool> botFuture;
    bool boardFlipped;

    // --- LAN multiplayer ---
    bool isNetworkGame;
    Piece::Color myColor;                 // local player's color (NoColor for offline games)
    std::unique_ptr<NetSession> net;
    bool reconnecting;
    // Mirror of the opponent's in-progress drag, for the live "ghost" piece.
    bool remoteDragging;
    std::pair<int,int> remoteOrigin;      // canonical square the opponent lifted from
    Piece::Type remotePieceType;
    Piece::Color remotePieceColor;
    float remoteCursorRowF;               // canonical board-space cursor (fractional)
    float remoteCursorColF;


    void drawBoard();
    void drawTime();
    void updateTime();
    void onPieceDraggingMoved(SDL_Event * event);
    void onPlacePieceDragging(SDL_Event * event);
    bool onInit();
    bool onExecute();
    void onEvent(SDL_Event * event);
    void onPawnPromotion(Move & move);
    void onPawnPromotionEvent(SDL_Event * event, int & input);
    void onCheckMate();
    void onStalemate();
    void onCleanup();
    void showMenu();
    void drawMenuScreen(const std::vector<std::string> & lines, const std::string & title, int hoveredButton = -1);
    int menuSelectFromScreen(const std::vector<std::string> & labels, const std::string & title, int optionCount);
    void setPieceDragging(SDL_Event * event);
    int convertYAxisToRow(int row);
    bool isPositionInMoves(int row, int col, vector<Move> & moves);

    // --- LAN multiplayer ---
    void setupNetworkGame();
    void hostGameFlow();
    void joinGameFlow();
    void finishNetworkSetup(TimeControl tc);
    bool waitingScreen(const std::string & code);
    std::string textInputScreen(const std::string & title, const std::string & prompt);
    void drawInfoScreen(const std::string & title, const std::vector<std::string> & lines);
    std::optional<NetProtocol::Assign> awaitAssign(int timeoutMs);
    void pollNetwork();
    void handleNetLine(const std::string & line);
    void applyRemoteMove(const NetProtocol::NetMove & nm);
    void applyPromotion(Move & move, Piece::Type type);
    void sendMoveMessage(Move & move);
    void sendStateResync();
    bool reconnectFlow();
    void drawRemoteGhost();
    static Piece::Type promoCharToType(char c);
    static char promoTypeToChar(Piece::Type t);

public:
    Chess();
    Chess(bool useBot, BotDifficulty level = Easy, Piece::Color botColor = Piece::Black, TimeControl tc = NoTimer);
    void play();


};


#endif //CHESSPP_CHESS_H
