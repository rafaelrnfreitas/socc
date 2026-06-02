cmake_minimum_required(VERSION 3.20)
project(compiler C)

set(CMAKE_C_STANDARD 99)

add_executable(cc
    src/main.c
    src/lexer.c
    src/vector.c
)

target_include_directories(cc PRIVATE ${CMAKE_SOURCE_DIR})