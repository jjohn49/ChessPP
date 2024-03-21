cmake_minimum_required(VERSION 3.24)
project(ChessPP)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)
Set(FETCHCONTENT_QUIET FALSE)

FetchContent_Declare(
        SDL2
        GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
        GIT_TAG release-2.26.3
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(SDL2)

FetchContent_Declare(
        SDL2_image
        GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
        GIT_TAG release-2.6.3
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
)

# START ADDITION
set(SDL2IMAGE_INSTALL OFF)
set(BUILD_SHARED_LIBS FALSE)
# END ADDITION

FetchContent_MakeAvailable(SDL2_image)

add_executable(ChessPP main.cpp 0.2/Source/Chess.cpp 0.2/Header/Chess.h 0.2/Source/Pieces/Piece.cpp 0.2/Header/Pieces/Piece.h 0.2/Source/Pieces/Pawn.cpp 0.2/Header/Pieces/Pawn.h 0.2/Source/Move.cpp 0.2/Header/Move.h 0.2/Source/Pieces/Bishop.cpp 0.2/Header/Pieces/Bishop.h
        0.2/Source/Pieces/Rook.cpp
        0.2/Header/Pieces/Rook.h
        0.2/Source/Pieces/Queen.cpp
        0.2/Header/Pieces/Queen.h
        0.2/Source/Pieces/Knight.cpp
        0.2/Header/Pieces/Knight.h
        0.2/Source/Pieces/King.cpp
        0.2/Header/Pieces/King.h
        0.2/Source/Board.cpp
        0.2/Header/Board.h
        src/Headers/Board.h
        src/Piece.cpp
        src/Piece.cpp
        src/Piece.h
)

target_link_libraries(ChessPP SDL2::SDL2main SDL2::SDL2-static SDL2_image::SDL2_image-static)












