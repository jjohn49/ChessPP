---
name: run
description: Build and run the ChessPP game
user_invocable: true
---

Build and launch the ChessPP chess game.

1. Build: `cmake --build build`
   - If the build directory isn't configured, run `cmake -B build -DCMAKE_BUILD_TYPE=Debug` first.
2. Run: `./build/ChessPP`
   - Note: The game requires SDL2 and opens a graphical window. It will not work in a headless environment.
3. The game is configured in `main.cpp`. Current settings: bot playing as Black at Hard difficulty.
