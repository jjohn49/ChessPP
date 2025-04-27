cmake_minimum_required(VERSION 3.5)

project(ChessPP)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_OSX_ARCHITECTURES arm64)

enable_language(OBJC)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

add_executable(ChessPP
        main.cpp
        src/Sources/Piece.cpp
        src/Headers/Piece.h
        src/Sources/Board.cpp
        src/Headers/Board.h
        src/Sources/Move.cpp
        src/Headers/Move.h
        src/Sources/Chess.cpp
        src/Headers/Chess.h
        src/Sources/Pawn.cpp
        src/Headers/Pawn.h
        src/Sources/King.cpp
        src/Headers/King.h
        src/Sources/Rook.cpp
        src/Headers/Rook.h
        src/Sources/Bishop.cpp
        src/Headers/Bishop.h
        src/Sources/Knight.cpp
        src/Headers/Knight.h
        src/Sources/Queen.cpp
        src/Headers/Queen.h
        src/Sources/Player.cpp
        src/Headers/Player.h
        src/Sources/Bot.cpp

)

target_link_libraries(ChessPP SDL2::SDL2 SDL2_image::SDL2_image SDL2_ttf::SDL2_ttf)












