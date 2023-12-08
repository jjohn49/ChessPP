cmake_minimum_required(VERSION 3.24)
project(ChessPP)

set(CMAKE_CXX_STANDARD 17)

add_executable(ChessPP main.cpp Source/Chess.cpp Header/Chess.h Source/Piece.cpp Header/Piece.h Source/Pawn.cpp Header/Pawn.h Source/Move.cpp Header/Move.h Source/Bishop.cpp Header/Bishop.h)
