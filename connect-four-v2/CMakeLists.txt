cmake_minimum_required(VERSION 3.9)
project(connect_four_v2)

set(CMAKE_CXX_STANDARD 11)

add_executable(connect_four_v2 main.cpp c4.cpp c4.h c4bot.cpp c4bot.h monte.c MonteCarloSolver.cpp MonteCarloSolver.h)