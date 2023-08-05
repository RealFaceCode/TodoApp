cmake_minimum_required(VERSION 3.20)
project(TodoApp)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXE_LINKER_FLAGS "-static")

add_executable(TodoApp main.cpp)
