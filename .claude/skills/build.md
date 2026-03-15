---
name: build
description: Build the ChessPP project (game and/or tests)
user_invocable: true
---

Build the ChessPP project using CMake.

1. Run `cmake -B build -DCMAKE_BUILD_TYPE=Debug` if the build directory hasn't been configured yet (check if `build/CMakeCache.txt` exists).
2. Run `cmake --build build` to build the main game executable.
3. Run `cmake --build build --target ChessPPTests` to build the test executable.
4. Report the build result (success or failure with errors).

If there are build errors, analyze the error messages and suggest fixes. Do not automatically apply fixes unless asked.
