cmake_minimum_required(VERSION 3.16)
project(rlang VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(r
    src/main.cpp
    src/lexer/lexer.cpp
    src/ast/ast.cpp
    src/parser/parser.cpp
    src/semantic/semantic.cpp
    src/bytecode/value.cpp
    src/bytecode/chunk.cpp
    src/bytecode/compiler.cpp
    src/bytecode/vm.cpp
    src/repl/repl.cpp
    src/gc/gc.cpp
)

target_include_directories(r PRIVATE src)
