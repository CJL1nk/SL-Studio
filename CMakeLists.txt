cmake_minimum_required(VERSION 4.1)
project(SL-Studio C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(SL-Studio
        src/main.c
        src/slstudio.c)

target_link_libraries(SL-Studio m)
