cmake_minimum_required(VERSION 3.23)
project(arcanoid)

set(CMAKE_CXX_STANDARD 17)

add_executable(arcanoid Project/game.cpp)

include_directories(Framework)
link_directories(Framework)

find_library(FrameworkReleaseLib FrameworkRelease HINTS Framework)

target_link_libraries(arcanoid ${FrameworkReleaseLib} SDL2 SDL2_image)