cmake_minimum_required(VERSION 3.24)
project(ChessPP)

set(CMAKE_CXX_STANDARD 17)

add_executable(ChessPP main.cpp Source/Piece.cpp Headers/Piece.h Source/Board.cpp Headers/Board.h)
