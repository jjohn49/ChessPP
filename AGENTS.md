# AGENTS.md — ChessPP Project Guide

## Project Overview

ChessPP is a C++ chess game with SDL2 rendering. It supports three modes, all chosen from the in-game start menu: local human-vs-human, human-vs-AI, and **online human-vs-human over a LAN**. The AI uses alpha-beta / negamax search with quiescence, a multi-threaded root search, make/unmake move optimization, and a Metal GPU batch evaluator on macOS (CPU fallback elsewhere). The LAN multiplayer is peer-to-peer over raw POSIX TCP: one player hosts a listener inside their own game process and the other connects with a short room code.

**Build**: CMake, C++17, clang++, links SDL2 + SDL2_image + SDL2_ttf + `Threads::Threads`. On macOS, also links Metal + Foundation for GPU evaluation.

**Run from the `build/` directory** — assets are loaded via relative paths (`../assets/...`), so the working directory must be `build/` (whose parent holds `assets/`).

## File Structure

```
ChessPP/
├── CMakeLists.txt
├── main.cpp                  # Entry point, initializes Chess game loop
├── src/
│   ├── Headers/
│   │   ├── Board.h           # 8x8 grid of shared_ptr<Piece>, evaluation, check detection, make/unmake
│   │   ├── Bot.h             # AI player: minimax, alpha-beta, negamax algorithms
│   │   ├── Chess.h           # Game loop, SDL rendering, turn management
│   │   ├── GPUEvaluator.h    # Abstract interface for batch board evaluation
│   │   ├── Move.h            # Stores old/new position + moving piece reference
│   │   ├── Piece.h           # Abstract base: Color, Type, position, getMoves()
│   │   ├── Player.h          # Human player: drag-drop input, move validation
│   │   ├── King.h            # Castling logic
│   │   ├── Queen.h
│   │   ├── Rook.h
│   │   ├── Bishop.h
│   │   ├── Knight.h
│   │   ├── Pawn.h            # En passant, promotion
│   │   ├── NetSession.h      # One LAN TCP connection: reader thread, inbox queue, reconnect
│   │   ├── NetProtocol.h     # PURE wire (de)serialization of multiplayer messages (no sockets/SDL)
│   │   └── RoomCode.h        # PURE LAN IPv4 <-> short room-code codec (no sockets/SDL)
│   ├── Sources/
│   │   ├── Board.cpp
│   │   ├── Bot.cpp
│   │   ├── Chess.cpp
│   │   ├── CPUEvaluator.cpp  # CPU fallback for batch evaluation (Linux/non-Metal)
│   │   ├── MetalEvaluator.mm # Metal GPU batch evaluation (macOS)
│   │   ├── Move.cpp
│   │   ├── Piece.cpp
│   │   ├── Player.cpp
│   │   ├── King.cpp
│   │   ├── Queen.cpp
│   │   ├── Rook.cpp
│   │   ├── Bishop.cpp
│   │   ├── Knight.cpp
│   │   ├── Pawn.cpp
│   │   ├── NetSession.cpp    # POSIX TCP: host/join, TCP_NODELAY, framing, reconnect
│   │   ├── NetProtocol.cpp
│   │   └── RoomCode.cpp
│   └── Shaders/
│       └── evaluate.metal    # Metal compute shader for batch board evaluation
├── tests/                    # No-SDL unit tests (TestFramework.h)
│   ├── test_board.cpp        # board state, make/unmake, deep copy
│   ├── test_pieces.cpp       # per-piece move generation
│   ├── test_bot.cpp          # search returns legal moves
│   ├── test_network.cpp      # RoomCode + NetProtocol round-trips
│   ├── test_check.cpp        # check detection during drag (lifted-king regression)
│   └── net_smoke.cpp         # manual loopback socket test -> NetSmoke target
```

`NetProtocol` and `RoomCode` are **pure** (no sockets/SDL) and live in
`CHESS_CORE_SOURCES` so the no-SDL `ChessPPTests` target can cover them.
`NetSession` (sockets + threads) links only into `ChessPP` and the `NetSmoke`
target.

### Class Hierarchy

- `Piece` (abstract) → `Pawn`, `Knight`, `Bishop`, `Rook`, `Queen`, `King`
- `Player` (human input) → `Bot` (AI search)
- `Board` — owns the 8x8 `shared_ptr<Piece>` grid, provides evaluation, check detection, and make/unmake move for search
- `Move` — value type storing old position, new position, and a `shared_ptr<Piece>` reference
- `Chess` — game loop, SDL rendering, menus, manages two players, the board, and (for online games) the `NetSession`
- `GPUEvaluator` (abstract) → `MetalEvaluator` (macOS), `CPUEvaluator` (fallback)
- `NetSession` — a single peer-to-peer TCP connection (host or join); owns the socket, a background reader thread, a mutex-guarded inbound message queue, and reconnect logic
- `NetProtocol` — pure free functions + POD structs (`NetMove`, `State`, …) for (de)serializing the newline-delimited wire format
- `RoomCode` — pure functions encoding/decoding a host's LAN IPv4 to/from a short Crockford-base32 room code

## LAN Multiplayer Architecture

Online play is **peer-to-peer over raw POSIX TCP**, all on the same local network.
There is no separate server binary: the host opens a listening socket inside its
own `Chess` process and also plays. Both peers run the identical engine.

**Canonical coordinates (load-bearing invariant).** `Board`'s `board[row][col]`
grid is in canonical coordinates (white home rank = row 0) and is identical on
both machines. `Chess::boardFlipped` only affects *rendering and input*, never the
stored grid. Therefore move/pickup coordinates are sent raw with no remapping;
only the floating drag cursor is converted per-orientation at render time.

**Transport — `NetSession`.** `host(port)` / `acceptOne(timeoutMs)` /
`join(ip, port, timeoutMs)` establish one connection with `TCP_NODELAY`. A reader
thread frames newline-delimited messages into a `std::mutex`-guarded `std::deque`;
the game loop drains it each frame via `poll()`. Sends happen on the main thread.
Disconnect is detected by `recv` returning 0 or a failed write. `reconnect()` has
the host re-accept on its existing listener and the joiner re-dial. Clean shutdown
is `shutdown()`+`join()`; the reader never touches `Board` or the renderer.

**Wire format — `NetProtocol`** (newline-delimited, space-separated, type token first):
`HELLO`, `ASSIGN <color> <tcSeconds>`, `MOVE <fromR> <fromC> <toR> <toC> <promo>`,
`PICKUP <r> <c>`, `DRAG <rowF> <colF>`, `DROP`, `STATE <turn> <wTime> <bTime> <count> <moves…>`,
`PING`/`PONG`. Piece-ish values are encoded as explicit chars (`Q/R/N`, `W/B`), never
raw enum ordinals, so the format is stable regardless of enum ordering.

**Room code — `RoomCode`.** Encodes the host's LAN IPv4 (+ a fixed port `kLanPort = 52525`)
into a ~7-char Crockford-base32 code. The join screen also accepts a raw `IP:port`.

**`Chess` integration:**
- *Menu*: `setupNetworkGame` → `hostGameFlow` / `joinGameFlow`. Host picks color + time control, shows a `waitingScreen(code)`; join uses `textInputScreen` (the only `SDL_TEXTINPUT` entry in the app). Handshake (`HELLO`/`ASSIGN`) sets `myColor`, time control, and `boardFlipped = (myColor == Black)`.
- *Turn gating*: in `onEvent`, input is accepted only when `!isNetworkGame || currentPlayer->getColor() == myColor`.
- *Move exchange*: a committed local move is sent via `sendMoveMessage`. Incoming moves go through `applyRemoteMove`, which rebuilds the move through the normal `Player::movePiece`/`canPieceMoveThere` path so castling/en-passant/promotion flags re-derive locally; the promotion *choice* is forced from the wire's `promo` char.
- *Live drag*: `PICKUP` on pickup, one `DRAG` per frame while dragging (canonical fractional coords derived from the render transform), `DROP` on cancel. The receiver hides the opponent's lifted piece at its origin and renders a ghost via `drawRemoteGhost`.
- *Reconnect/resync*: a periodic `PING` surfaces a broken link; `reconnectFlow` retries, then the **host** is authoritative and pushes a full `STATE` (its move history). The joiner replays it through `Board::loadFromMoves`, which reconstructs `moveHistory`, `hasMoved` flags, and en-passant legality identically to live play. `Board::recordPromotion` back-fills the chosen promotion piece onto the last history entry so resync preserves under-promotions.

## Coding Rules

These rules MUST be followed when writing or modifying code in this project.

### Formatting

- **Use tabs for indentation, never spaces.** All indentation must be tab characters.
- Place opening braces on the same line as the statement (K&R style).
- Use a single blank line between function definitions. No trailing blank lines at end of file.

### Initialization

- **Always use universal (brace) initialization** where possible: `Type name{args}` instead of `Type name(args)` or `Type name = value`.
  - Example: `int x{5};`, `vector<int> v{1, 2, 3};`, `Bot b{Easy, Piece::White, &board};`
  - Exception: when brace initialization would invoke `std::initializer_list` constructor unintentionally (e.g. `vector<int>{10}` creates a 1-element vector, not a 10-element one — use `vector<int>(10)` in that case).

### File Organization

- **One class per file.** Each class gets its own `.h` header and `.cpp` source file.
- Headers go in `src/Headers/`, source files go in `src/Sources/`.
- Metal/GPU shaders go in `src/Shaders/`.
- Objective-C++ files (`.mm`) are used only when interfacing with Apple frameworks (Metal, Foundation).
- Header files use `#ifndef`/`#define`/`#endif` include guards with the pattern `CHESSPP_CLASSNAME_H`.

### Object-Oriented Design

- **Use proper inheritance.** The `Piece` class is the abstract base for all piece types (`Pawn`, `Knight`, `Bishop`, `Rook`, `Queen`, `King`). New piece behavior goes in the appropriate subclass, not in `Board` or `Bot`.
- `Player` is the base for `Bot`. Player handles human input; Bot overrides `move()` with AI search.
- `GPUEvaluator` is the abstract base for batch evaluation backends (`MetalEvaluator`, `CPUEvaluator`). New GPU backends implement this interface.
- Virtual methods should be marked `override` in derived classes.
- Prefer `shared_ptr<Piece>` for piece ownership (pieces are shared across the board grid and move objects).

### Naming

- Classes: `PascalCase` (e.g. `Board`, `GPUEvaluator`)
- Methods: `camelCase` (e.g. `getMinMaxMove`, `makeSearchMove`)
- Member variables: `camelCase` (e.g. `moveHistory`, `generatingAttacks`)
- Enums: `PascalCase` for both the enum name and values (e.g. `Piece::Type::King`, `BotDifficulty::Hard`)
- Constants: `camelCase` (e.g. `pieceValues`)

### C++ Standard

- Target **C++17**. Use `std::optional`, `std::shared_ptr`, structured bindings, etc. freely.
- Prefer `<ctime>` over `<time.h>`, `<cstdlib>` over `<stdlib.h>` (C++ headers over C headers).
- Do not add unused includes. Remove includes that are no longer needed after refactoring.

## Remaining Inefficiencies

### No transposition table

The same board position can be reached via different move orders. Without a transposition table, identical positions are re-evaluated thousands of times.

### Thread-per-move at root level

`getMinMaxMove` spawns one `std::async` thread per root move — potentially 30+ threads simultaneously. A bounded thread pool would reduce context switching overhead.

### No iterative deepening

Hard-coded search depth with no time management. The bot cannot adapt search depth to position complexity or time constraints.

### King value `20000` in evaluation

While reduced from the previous `1000000000000`, the king value is still very large relative to other pieces and dominates the evaluation.

## Future Algorithm Improvements

### 1. Iterative Deepening

Search to depth 1, then depth 2, then depth 3, etc., until time runs out. The best move from each iteration seeds move ordering for the next, dramatically improving alpha-beta cutoffs. Also provides a "best move so far" that can be returned if time expires.

### 2. Transposition Tables (Zobrist Hashing)

Assign a random 64-bit number to each (piece, square) combination. XOR them together to get a hash of any board position. Store evaluated positions in a hash table keyed by this hash. When the same position is reached via a different move order, look up the cached result instead of re-searching. This avoids exponential re-work in positions with many transpositions.

### 3. Quiescence Search — IMPLEMENTED

`Bot::quiesce` now resolves outstanding captures past the search horizon. The
description below is retained as background.

At leaf nodes of the main search, don't just return the static evaluation — continue searching capture moves until the position is "quiet" (no captures available). This prevents the "horizon effect" where the bot stops searching right before a piece is captured, leading to wildly inaccurate evaluations. Typical implementation: at depth 0, generate only capture moves and recurse with quiescence search instead of the main search.

### 4. Move Ordering Improvements

Better move ordering means more alpha-beta cutoffs and faster search. Key techniques:

- **MVV-LVA (Most Valuable Victim - Least Valuable Attacker)**: Order captures by the value of the captured piece minus the attacker's value. Capturing a queen with a pawn is searched before capturing a pawn with a queen.
- **Killer Moves**: Store moves that caused beta cutoffs at each depth. Try these moves early at the same depth in sibling nodes — they often cause cutoffs again.
- **History Heuristic**: Track how often each move causes cutoffs across the entire search. Moves that frequently cause cutoffs are searched earlier.

### 5. Principal Variation Search (PVS)

Search the first move (expected best) with a full alpha-beta window. Search all remaining moves with a **null window** (alpha, alpha+1). If a move scores above alpha with the null window, re-search it with the full window. When move ordering is good, most re-searches are avoided, making PVS significantly faster than standard alpha-beta.

### 6. Null Move Pruning

Before searching a position normally, try "passing" (making a null move — letting the opponent move twice). Search this with reduced depth (typically depth-3). If the result is still >= beta (i.e., the position is so good that even skipping a turn doesn't help the opponent), prune the entire branch. Provides huge speedups but must be disabled in zugzwang positions (where passing would actually be beneficial).

### 7. Late Move Reductions (LMR)

After searching the first few moves at full depth, reduce the search depth for later moves (which, if move ordering is good, are likely bad). If a reduced-depth search returns a surprisingly good score, re-search at full depth. Combined with good move ordering, LMR allows searching 2-3 plies deeper within the same time budget.

### 8. Neural Network Evaluation (NNUE)

Replace the hand-crafted `evaluate()` function with a neural network trained on millions of positions. NNUE (Efficiently Updatable Neural Network) is used in modern Stockfish. The key insight is that the network weights can be incrementally updated when a move is made, making evaluation nearly as fast as a hand-crafted function but far more accurate. Requires a training pipeline and careful integration with the search.

### 9. Monte Carlo Tree Search (MCTS)

Instead of exhaustively searching to a fixed depth, MCTS uses statistical sampling. Repeatedly: (1) select a promising node using UCB1, (2) expand it by adding a child, (3) simulate a random playout to the end of the game, (4) backpropagate the result. Over thousands of iterations, the tree converges on the best move. Used by AlphaZero in combination with a neural network policy/value head. Better suited for very deep strategic planning but requires many more iterations than minimax for tactical positions.

## Testing

Unit tests use a tiny header-only framework (`tests/TestFramework.h`): `TEST(name){...}`
auto-registers and `tests/test_main.cpp` runs everything. Tests link **only core
(no-SDL) sources** (`CHESS_CORE_SOURCES` in `CMakeLists.txt`), so anything tested
must avoid SDL — keep rules, board state, the wire protocol, and the room-code codec
in pure classes.

```bash
cd build
make ChessPPTests && ./ChessPPTests   # full unit suite
make NetSmoke      && ./NetSmoke       # manual loopback socket check (real TCP/threads)
```

When adding networking logic, put pure (de)serialization in `NetProtocol`/`RoomCode`
(unit-testable) and keep socket/thread code in `NetSession` (covered by `NetSmoke`,
not the unit suite). The check-detection regression in `tests/test_check.cpp` is a
reminder that interactive validation lifts the moving piece off the grid — code that
looks up king position must consult the board, not a stale cache.

## Knowledge Vault

Use `/vault` to read, write, search, and organize the Obsidian knowledge vault. The vault lives at `/Users/hugh/Library/Mobile Documents/iCloud~md~obsidian/Documents/Jack's Obsidian Vault/` and syncs to the user's phone. Start from `Home.md` for navigation.

**Relevant notes for this project:**
- `Projects/ChessPP.md` — project overview, current state, coding rules
- `Knowledge/CPP/Chess Engine Algorithms.md` — implemented and planned algorithms
- `Knowledge/CPP/ChessPP Bug Registry.md` — 16 fixed bugs, 6 open improvements

**Auto-trigger:** At session start, read `Projects/ChessPP.md` for current state. After completing a major milestone (bug fix, new algorithm, performance improvement), use `/vault` to update the corresponding notes.
