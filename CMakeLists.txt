cmake_minimum_required(VERSION 3.24)
project(ChessPP)

set(CMAKE_CXX_STANDARD 17)

add_executable(ChessPP main.cpp 1.0/Source/Pieces/Piece.cpp 1.0/Headers/Pieces/Piece.h 1.0/Source/Board.cpp 1.0/Headers/Board.h 1.0/Source/Move.cpp 1.0/Headers/Move.h 1.0/Source/MovesForPieces.cpp 1.0/Headers/MovesForPieces.h 1.0/Source/Chess.cpp 1.0/Headers/Chess.h Source/Chess.cpp Header/Chess.h Source/Piece.cpp Header/Piece.h Source/Pawn.cpp Header/Pawn.h Source/Move.cpp Header/Move.h)
