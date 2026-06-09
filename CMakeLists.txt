cmake_minimum_required(VERSION 3.20)
project(compiler C)

set(CMAKE_C_STANDARD 99)

add_executable(
    socc
    src/main.c
    src/lexer.c
    src/vector.c
    src/ast.c
    src/parser.c
    src/asmgen.c
)

target_include_directories(socc PRIVATE ${CMAKE_SOURCE_DIR})