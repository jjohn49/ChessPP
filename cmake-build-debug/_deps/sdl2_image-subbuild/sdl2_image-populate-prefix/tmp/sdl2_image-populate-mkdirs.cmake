# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/hugh/Projects/jj-git-repos/ChessPP/cmake-build-debug/_deps/sdl2_image-src"
  "/home/hugh/Projects/jj-git-repos/ChessPP/cmake-build-debug/_deps/sdl2_image-build"
  "/home/hugh/Projects/jj-git-repos/ChessPP/cmake-build-debug/_deps/sdl2_image-subbuild/sdl2_image-populate-prefix"
  "/home/hugh/Projects/jj-git-repos/ChessPP/cmake-build-debug/_deps/sdl2_image-subbuild/sdl2_image-populate-prefix/tmp"
  "/home/hugh/Projects/jj-git-repos/ChessPP/cmake-build-debug/_deps/sdl2_image-subbuild/sdl2_image-populate-prefix/src/sdl2_image-populate-stamp"
  "/home/hugh/Projects/jj-git-repos/ChessPP/cmake-build-debug/_deps/sdl2_image-subbuild/sdl2_image-populate-prefix/src"
  "/home/hugh/Projects/jj-git-repos/ChessPP/cmake-build-debug/_deps/sdl2_image-subbuild/sdl2_image-populate-prefix/src/sdl2_image-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/hugh/Projects/jj-git-repos/ChessPP/cmake-build-debug/_deps/sdl2_image-subbuild/sdl2_image-populate-prefix/src/sdl2_image-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/hugh/Projects/jj-git-repos/ChessPP/cmake-build-debug/_deps/sdl2_image-subbuild/sdl2_image-populate-prefix/src/sdl2_image-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
