cmake_minimum_required(VERSION 3.24)
project(ChessPP)

set(CMAKE_CXX_STANDARD 17)

#find_library(SDL2_LIBRARY NAME SDL2 REQUIRED)
find_package(SDL2)


set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

find_package(SDL2TTF REQUIRED)
find_package(SDL2_IMAGE REQUIRED >= 2.0.0)


message ([STATUS] "SDL2_INCLUDE_DIR" ${SDL2_INCLUDE_DIRS})
message ([STATUS] "SDL2_LIBRARY" ${SDL2_LIBRARY})

message ([STATUS] "SDL2_IMAGE_INCLUDE_DIR" ${SDL2_IMAGE_INCLUDE_DIR})
message ([STATUS] "SDL2_IMAGE_LIBRARY" ${SDL2_IMAGE_LIBRARY})

message ([STATUS] "SDL2TTF_INCLUDE_DIR" ${SDL2TTF_INCLUDE_DIR})
message ([STATUS] "SDL2TTF_LIBRARY" ${SDL2TTF_LIBRARY})


#include(FetchContent)
#Set(FETCHCONTENT_QUIET FALSE)
#
#FetchContent_Declare(
#        SDL2
#        GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
#        GIT_TAG release-2.26.3
#        GIT_SHALLOW TRUE
#        GIT_PROGRESS TRUE
#)
#FetchContent_MakeAvailable(SDL2)
#
#FetchContent_Declare(
#        SDL2_image
#        GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
#        GIT_TAG release-2.6.3
#        GIT_SHALLOW TRUE
#        GIT_PROGRESS TRUE
#)
#
## START ADDITION
#set(SDL2IMAGE_INSTALL OFF)
#set(BUILD_SHARED_LIBS FALSE)
## END ADDITION
#
#FetchContent_MakeAvailable(SDL2_image)
#
#FetchContent_Declare(
#        SDL2_ttf
#        GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf.git
#        GIT_TAG release-2.22.0
#        GIT_SHALLOW TRUE
#        GIT_PROGRESS TRUE
#)
#FetchContent_MakeAvailable(SDL2_ttf)

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
        src/Headers/Bot.h
)



include_directories(ChessPP ${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIR} ${SDL2TTF_INCLUDE_DIR})
target_link_libraries(ChessPP ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARY} ${SDL2TTF_LIBRARY})


#target_link_libraries(ChessPP SDL2::SDL2main SDL2::SDL2-static SDL2_image::SDL2_image-static SDL2_ttf::SDL2_ttf-static)













