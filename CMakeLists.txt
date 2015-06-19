cmake_minimum_required(VERSION 3.2)
project(MiniProject1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
set(SOURCE_FILES data_parser.cpp Lexer.cpp Lexer.h)
add_executable(MiniProject1 ${SOURCE_FILES} data_parser.cpp Lexer.cpp Lexer.h)