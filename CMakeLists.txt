cmake_minimum_required(VERSION 4.1)
project(SL-Studio CXX)

set(CMAKE_CXX_STANDARD 20)

include_directories(.)

add_executable(SL-Studio
        src/main.cpp
        src/slstudio.cpp
        src/slstudio.hpp)

target_link_libraries(SL-Studio m)
