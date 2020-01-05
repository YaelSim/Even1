cmake_minimum_required(VERSION 3.15)
project(untitled6)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS -pthread)
add_executable(untitled6 main.cpp lexer.cpp lexer.h Command.cpp Command.h Parser.cpp Parser.h SymbolTable.h SymbolTable.cpp VarObject.cpp VarObjects.h ex1.h ex1.cpp Expression.h Interpreter.cpp Interpreter.h)