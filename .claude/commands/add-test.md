---
name: add-test
description: Create a new unit test for ChessPP
user_invocable: true
---

Create a new unit test for the ChessPP project. The user will describe what they want to test.

Follow these rules when writing tests:

1. **Add tests to the appropriate existing file:**
   - `tests/test_board.cpp` — Board state, evaluation, move execution, make/unmake
   - `tests/test_pieces.cpp` — Piece move generation, special moves (castling, en passant, promotion)
   - `tests/test_bot.cpp` — Bot search, move selection, board corruption checks

2. **If a new test file is needed**, create it in `tests/` and add it to `CMakeLists.txt` under the `ChessPPTests` target. Include `TestFramework.h`.

3. **Use the test macros from `tests/TestFramework.h`:**
   - `TEST(TestName)` — define a test
   - `ASSERT_TRUE(expr)`, `ASSERT_FALSE(expr)`
   - `ASSERT_EQ(a, b)`, `ASSERT_NE(a, b)`
   - `ASSERT_GT(a, b)`, `ASSERT_LT(a, b)`
   - `ASSERT_NOT_NULL(ptr)`

4. **Follow the coding rules:** tabs for indentation, universal initialization.

5. **Avoid constructing boards from scratch** (clearing all squares and placing pieces manually). The `Board()` constructor initializes piece tracking vectors that `deepCopy()` and `Player::isInCheck()` depend on. Instead, modify a default-constructed board minimally.

6. After writing the test, build and run: `cmake --build build --target ChessPPTests && ./build/ChessPPTests`
