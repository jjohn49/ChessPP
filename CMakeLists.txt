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

add_executable(ChessPP main.cpp Source/Chess.cpp Header/Chess.h Source/Pieces/Piece.cpp Header/Pieces/Piece.h Source/Pieces/Pawn.cpp Header/Pieces/Pawn.h Source/Move.cpp Header/Move.h Source/Pieces/Bishop.cpp Header/Pieces/Bishop.h
        Source/Pieces/Rook.cpp
        Header/Pieces/Rook.h
        Source/Pieces/Queen.cpp
        Header/Pieces/Queen.h
        Source/Pieces/Knight.cpp
        Header/Pieces/Knight.h
        Source/Pieces/King.cpp
        Header/Pieces/King.h
        Source/Board.cpp
        Header/Board.h)

target_link_libraries(ChessPP SDL2::SDL2main SDL2::SDL2-static SDL2_image::SDL2_image-static)












