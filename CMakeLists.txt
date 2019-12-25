cmake_minimum_required(VERSION 3.15)
project(Even1)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS -pthread)

add_executable(Even1 main.cpp lexer.cpp Command.cpp SymbolTable.cpp Parser.cpp lexer.h Parser.h Command.h VarObjects.h VarObjects.cpp ex1.h ex1.cpp Expression.h Interpreter.cpp Interpreter.h)