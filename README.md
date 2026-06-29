# ChessPP

A full-featured chess game written in C++ with SDL2 — play a friend on the same
screen, play **online over your local network**, or take on an AI bot with
selectable difficulty and chess clocks.

<img width="806" alt="ChessPP board" src="https://github.com/jjohn49/ChessPP/assets/98774069/8eeff2b9-ed20-45f3-b1da-608c91eb3252">

## Features

- ♟️ **Complete chess rules** — legal move generation, check/checkmate/stalemate, castling, en passant, and pawn promotion.
- 🖱️ **Drag-and-drop UI** — click to pick up a piece, legal destinations highlight, click to place. Promotion is chosen from an on-screen popup (no console needed).
- 🤖 **AI bots** — six difficulty levels (Random → Expert) using alpha-beta / negamax search with quiescence, a multi-threaded root search, and a Metal GPU batch evaluator on macOS.
- ⏱️ **Chess clocks** — Bullet, Blitz, 5 min, Rapid, 20 min, Classical, or no timer.
- 🌐 **Online multiplayer over LAN** — host a game, share a short room code, and your friend joins from another machine on the same Wi-Fi. Includes a live "ghost" piece that mirrors your opponent's drag in real time, and automatic reconnect if the connection drops.
- 🧭 **In-game menus** — everything (mode, difficulty, color, time control, host/join) is chosen from the start screen.

## Requirements

- A C++17 compiler (clang++ recommended) and **CMake ≥ 3.24**
- **SDL2**, **SDL2_image**, and **SDL2_ttf**

Install the SDL libraries:

```bash
# macOS (Homebrew)
brew install sdl2 sdl2_image sdl2_ttf cmake

# Debian / Ubuntu
sudo apt install libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev cmake
```

On macOS the build also links the **Metal** and **Foundation** frameworks for GPU
board evaluation (this is automatic; a CPU fallback is used everywhere else).

## Build & Run

The game loads its assets from `../assets`, so build and run from the `build/`
directory:

```bash
mkdir -p build && cd build
cmake ..
make ChessPP
./ChessPP
```

## How to Play

When the game starts you'll step through a few menu screens (click a button or
press the matching number key):

1. **Game Mode** — *Play vs Human (Local)*, *Play vs Human (Online)*, or *Play vs Bot*.
2. **Bot settings** (vs Bot only) — difficulty (Random, Super Easy, Easy, Intermediate, Hard, Expert) and which color you play.
3. **Time Control** — No Timer, Bullet (1 min), Blitz (3 min), 5 Minutes, Rapid (10 min), 20 Minutes, or Classical (30 min).

**Making a move:** click a piece to pick it up — its legal destinations light up
red — then click a square to drop it. If a pawn reaches the last rank, a popup
lets you promote to a Queen, Rook, or Knight. The game ends automatically on
checkmate, stalemate, or flag-fall and shows the result.

## Online Multiplayer (LAN)

Both players must be on the **same local network** (e.g. the same Wi-Fi).

**To host:**
1. Choose *Play vs Human (Online)* → *Host Game*.
2. Pick your color (White, Black, or Random) and a time control.
3. A **room code** appears (e.g. `1FRD5EM`) along with your IP. Share the code.
   - On macOS you may be asked to *allow incoming network connections* the first time — allow it.
4. The game begins as soon as your opponent joins.

**To join:**
1. Choose *Play vs Human (Online)* → *Join Game*.
2. Type the room code your host gave you (or an `IP:port` directly) and press Enter.

Each player sees the board from their own side, moves are kept in sync, and you'll
see your opponent's piece glide as they drag it. If the connection drops, the game
shows *Reconnecting…* and re-syncs automatically; if it can't, it returns to the menu.

## Rules Showcase

### 1. Knights Jumping Pieces

<img width="1197" alt="Knight move" src="https://github.com/jjohn49/ChessPP/assets/98774069/fe510834-8878-47b9-9c98-e5cb79532f7a">

### 2. En Passant

<img width="1196" alt="En passant before" src="https://github.com/jjohn49/ChessPP/assets/98774069/aeb8894a-ec19-4a16-b4be-a7e3664caaa4">
<img width="1198" alt="En passant after" src="https://github.com/jjohn49/ChessPP/assets/98774069/e36cbbd0-a19b-4c62-a6ca-12ca4eb067ab">

### 3. Pawn Promotion

<img width="1199" alt="Promotion before" src="https://github.com/jjohn49/ChessPP/assets/98774069/7cf99901-066c-45ed-81a7-f8f9484e1cb4">
<img width="1202" alt="Promotion after" src="https://github.com/jjohn49/ChessPP/assets/98774069/2cd3a9c8-05a8-4451-8773-1bfe66e6d687">

### 4. Castling

<img width="1197" alt="Castling before" src="https://github.com/jjohn49/ChessPP/assets/98774069/d2bf9aac-0508-4a1b-8e5e-f0c61df2d282">
<img width="1200" alt="Castling after" src="https://github.com/jjohn49/ChessPP/assets/98774069/37fc6a99-83ae-487d-a170-63282d3fc7e2">

## Tests

Core logic (move rules, board state, the networking protocol, and check
detection) has unit tests with no SDL dependency:

```bash
cd build
make ChessPPTests && ./ChessPPTests
```

There is also a `NetSmoke` target (`make NetSmoke && ./NetSmoke`) that exercises
the TCP networking layer over loopback.
