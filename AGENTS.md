# AGENTS.md — ChessPP Project Guide

## Project Overview

ChessPP is a C++ chess game with SDL2 rendering and AI bots using minimax/alpha-beta pruning. The `parallelProcessing` branch features multi-threaded root-level search, make/unmake move optimization, and a Metal GPU batch evaluator for macOS.

**Build**: CMake, C++17, clang++, links SDL2 + SDL2_image + SDL2_ttf. On macOS, also links Metal + Foundation for GPU evaluation.

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
│   │   └── Pawn.h            # En passant, promotion
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
│   │   └── Pawn.cpp
│   └── Shaders/
│       └── evaluate.metal    # Metal compute shader for batch board evaluation
```

### Class Hierarchy

- `Piece` (abstract) → `Pawn`, `Knight`, `Bishop`, `Rook`, `Queen`, `King`
- `Player` (human input) → `Bot` (AI search)
- `Board` — owns the 8x8 `shared_ptr<Piece>` grid, provides evaluation, check detection, and make/unmake move for search
- `Move` — value type storing old position, new position, and a `shared_ptr<Piece>` reference
- `Chess` — game loop, SDL rendering, manages two players and the board
- `GPUEvaluator` (abstract) → `MetalEvaluator` (macOS), `CPUEvaluator` (fallback)

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

`getMinMaxMove` and `getNegaMaxMove` spawn one `std::async` thread per root move — potentially 30+ threads simultaneously. A bounded thread pool would reduce context switching overhead.

### No iterative deepening

Hard-coded search depth with no time management. The bot cannot adapt search depth to position complexity or time constraints.

### King value `20000` in evaluation

While reduced from the previous `1000000000000`, the king value is still very large relative to other pieces and dominates the evaluation.

## Future Algorithm Improvements

### 1. Iterative Deepening

Search to depth 1, then depth 2, then depth 3, etc., until time runs out. The best move from each iteration seeds move ordering for the next, dramatically improving alpha-beta cutoffs. Also provides a "best move so far" that can be returned if time expires.

### 2. Transposition Tables (Zobrist Hashing)

Assign a random 64-bit number to each (piece, square) combination. XOR them together to get a hash of any board position. Store evaluated positions in a hash table keyed by this hash. When the same position is reached via a different move order, look up the cached result instead of re-searching. This avoids exponential re-work in positions with many transpositions.

### 3. Quiescence Search

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
