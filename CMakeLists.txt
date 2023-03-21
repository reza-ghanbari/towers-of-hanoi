cmake_minimum_required(VERSION 3.24)
project(Towers_of_Hanoi_Solver)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "-O3 -fopenmp")

add_executable(Towers_of_Hanoi_Solver main.cpp model/State.cpp inc/State.h utils/Constants.h model/Heuristic.cpp inc/Heuristic.h utils/Types.h model/Solver.cpp inc/Solver.h)
