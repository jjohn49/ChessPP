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


)

target_link_libraries(ChessPP SDL2::SDL2main SDL2::SDL2-static SDL2_image::SDL2_image-static)












