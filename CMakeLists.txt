cmake_minimum_required(VERSION 3.30)
project(ggtoolbox)

set(CMAKE_CXX_STANDARD 20)

add_executable(ggtoolbox main.cpp
        nbs.h
        nbsapi.h)
target_link_libraries(ggtoolbox raylib opengl32 gdi32 winmm)