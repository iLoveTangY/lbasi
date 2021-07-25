cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_CXX_STANDARD 17)

project(interpreter)

set(SRC ./main.cpp
        ./lexer.cpp
        ./interpreter.cpp
        ./parser.cpp
        ./token.cpp
        ./symbol.cpp
        ./error.cpp
    )

add_executable(interpreter ${SRC})
