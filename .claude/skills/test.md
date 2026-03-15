---
name: test
description: Build and run the ChessPP unit tests
user_invocable: true
---

Build and run the ChessPP unit test suite.

1. Build the test target: `cmake --build build --target ChessPPTests`
   - If the build directory isn't configured, run `cmake -B build -DCMAKE_BUILD_TYPE=Debug` first.
2. Run the tests: `./build/ChessPPTests`
3. Report the results: how many passed, how many failed, and the names of any failing tests.
4. If any tests fail, read the failing test code and the relevant source code to diagnose the failure. Suggest a fix but do not apply it unless asked.

Tests are in the `tests/` directory:
- `test_board.cpp` — Board initialization, evaluation, make/unmake, deep copy
- `test_pieces.cpp` — Individual piece move generation
- `test_bot.cpp` — Bot search algorithms, move ordering, board corruption checks

The test framework is a minimal header-only framework in `tests/TestFramework.h`. Tests use `TEST(name)` macros and `ASSERT_*` macros.
