# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/Users/hugh/Desktop/jj-git-repos/ChessPP/cmake-build-release/_deps/sdl2-src"
  "/Users/hugh/Desktop/jj-git-repos/ChessPP/cmake-build-release/_deps/sdl2-build"
  "/Users/hugh/Desktop/jj-git-repos/ChessPP/cmake-build-release/_deps/sdl2-subbuild/sdl2-populate-prefix"
  "/Users/hugh/Desktop/jj-git-repos/ChessPP/cmake-build-release/_deps/sdl2-subbuild/sdl2-populate-prefix/tmp"
  "/Users/hugh/Desktop/jj-git-repos/ChessPP/cmake-build-release/_deps/sdl2-subbuild/sdl2-populate-prefix/src/sdl2-populate-stamp"
  "/Users/hugh/Desktop/jj-git-repos/ChessPP/cmake-build-release/_deps/sdl2-subbuild/sdl2-populate-prefix/src"
  "/Users/hugh/Desktop/jj-git-repos/ChessPP/cmake-build-release/_deps/sdl2-subbuild/sdl2-populate-prefix/src/sdl2-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/Users/hugh/Desktop/jj-git-repos/ChessPP/cmake-build-release/_deps/sdl2-subbuild/sdl2-populate-prefix/src/sdl2-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/Users/hugh/Desktop/jj-git-repos/ChessPP/cmake-build-release/_deps/sdl2-subbuild/sdl2-populate-prefix/src/sdl2-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
