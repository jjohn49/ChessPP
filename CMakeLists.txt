cmake_minimum_required(VERSION 3.24)
project(ChessPP)

set(CMAKE_CXX_STANDARD 17)

add_executable(ChessPP main.cpp Source/Pieces/Piece.cpp Headers/Pieces/Piece.h Source/Board.cpp Headers/Board.h Source/Move.cpp Headers/Move.h Source/MovesForPieces.cpp Headers/MovesForPieces.h Source/Chess.cpp Headers/Chess.h)
