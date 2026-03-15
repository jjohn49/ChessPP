# ChessPP Kanban Board

## Done

- [x] **BUG-1: `isColorInCheck()` checks wrong color** (Board.cpp)
  - Changed `== color` to `!= color`. Check/checkmate detection now works.

- [x] **BUG-2: Alpha-beta initial values swapped** (Bot.cpp)
  - Swapped to `alpha=-huge, beta=+huge`. Bot actually searches the tree now.

- [x] **BUG-3: `bestVal = alpha` instead of `bestVal = score`** (Bot.cpp)
  - Changed to `bestVal = score`. maxAlphaBeta tracks real best value.

- [x] **BUG-4: NegaMax corrupts board state via shared_ptr** (Bot.cpp)
  - Added `deepCopy()` in negaMaxAlgo and getNegaMaxMove. No more shared_ptr mutation.

- [x] **BUG-5: `Player::isInCheck()` shallow copy** (Player.cpp)
  - Now uses `board->deepCopy()` and operates on copied pieces only.

- [x] **BUG-6: `Bot::move()` clears old position prematurely** (Bot.cpp)
  - Moved old-position clearing into `Board::movePiece()` where it belongs.

- [x] **BUG-7: Castling rook indices swapped** (King.cpp)
  - King-side now uses `colorOffset+1`, queen-side uses `colorOffset`.

- [x] **BUG-8: Castling checks king's own square for nullptr** (King.cpp)
  - Removed `squares[0]` from emptiness check. Added 3rd square check for queen-side.

- [x] **BUG-9: `maxAlphaBeta` generates moves for wrong color** (Bot.cpp)
  - Was generating same-color moves instead of opponent's. Fixed to alternate correctly.

- [x] **BUG-10: `getColorsKingPosition` crashes on missing king** (Board.cpp)
  - Added bounds check + board scan fallback when king not in `kings` vector.

- [x] **BUG-11: Infinite recursion in castling/check detection** (King.cpp, Board.cpp)
  - Added `generatingAttacks` flag to skip castling moves during attack checks.

- [x] **BUG-12: `nullopt.value()` crash in alpha-beta** (Bot.cpp)
  - Both `maxAlphaBeta` and `minAlphaBeta` now skip `nullopt` results with `continue`.

- [x] **BUG-13: Thread-per-move in `getMinMaxMove`** (Bot.cpp)
  - Replaced with sequential search. Threads couldn't share pruning info so were slower.

- [x] **BUG-14: No stalemate detection** (Player.cpp, Chess.cpp)
  - `isCheckMated()` now requires king to be in check. Added `isStalemated()` for draws.

- [x] **BUG-15: `isCheckMated()` called on every SDL event** (Chess.cpp)
  - Now only checked after a move is actually made.

- [x] **BUG-16: Deprecated `minMaxAlgo` uses wrong board** (Bot.cpp)
  - Fixed to use `copyBoard` instead of `board` for move generation and recursion.

- [x] **PERF-1: Move sorting is ascending (worst first)** (Bot.cpp)
  - Changed `less<float>()` to `greater<float>()`. Best moves searched first.

- [x] **PERF-2: `evaluate()` rebuilds pointsPerPiece map every call** (Board.cpp)
  - Made `static const`. Also reduced King value from 1e12 to 20000.

- [x] **PERF-3: Remove duplicate `sortMoves` function** (Bot.cpp, Bot.h)
  - Deleted dead code duplicate of `getAllMovesSorted()`.

- [x] **BUG-EXTRA: `int bestVal = 1000000000000` overflow** (Bot.cpp)
  - Changed to `double` to fix implicit truncation warning.

## In Progress

_None_

## Todo

### Future Improvements

- [ ] **IMP-1: Add piece-square tables to evaluation**
  - 64-element array per piece type for positional awareness.

- [ ] **IMP-2: Implement make/unmake move instead of deep copy**
  - Eliminates millions of allocations per search.

- [ ] **IMP-3: Use thread pool instead of thread-per-move**
  - Cap at `hardware_concurrency()` workers.

- [ ] **IMP-4: Add transposition table with Zobrist hashing**
  - Cache evaluated positions, avoid re-searching identical states.

- [ ] **IMP-5: Add quiescence search**
  - Continue searching captures at leaf nodes to prevent horizon effect.

- [ ] **IMP-6: Implement iterative deepening**
  - Search depth 1, 2, 3... with time cutoff. Better move ordering and time control.
