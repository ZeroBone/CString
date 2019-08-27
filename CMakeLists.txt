cmake_minimum_required(VERSION 3.14)
project(cstring C)

set(CMAKE_C_STANDARD 99)

add_executable(cstring main.c src/string.c src/string.h)