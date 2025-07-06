cmake_minimum_required(VERSION 3.30)
project(ggtoolbox)

set(CMAKE_CXX_STANDARD 20)

if(CMAKE_GENERATOR STREQUAL "Ninja")
    set(CMAKE_UNITY_BUILD ON)
    set(CMAKE_UNITY_BUILD_BATCH_SIZE 8)
    set(PCH_HEADER nbs.h)
    add_executable(ggtoolbox main.cpp nbs.h nbsapi.h)
    if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.16")
        target_precompile_headers(ggtoolbox PRIVATE ${PCH_HEADER})
    endif()
else()
    add_executable(ggtoolbox main.cpp nbs.h nbsapi.h)
endif()

target_link_libraries(ggtoolbox raylib opengl32 gdi32 winmm)

