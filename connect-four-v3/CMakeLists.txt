cmake_minimum_required(VERSION 3.9)
project(connect_four_v3)

set(CMAKE_CXX_STANDARD 11)

add_executable(connect_four_v3 main.cpp c4bot.cpp c4bot.h c4.cpp c4.h Solver.cpp Solver.h)