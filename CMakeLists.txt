cmake_minimum_required(VERSION 3.14)
project(prir)

set(CMAKE_CXX_STANDARD 14 )
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -fopenmp)

add_executable(prir main.cpp Utils.cpp Utils.h StandScaler.cpp StandScaler.h NormScaler.cpp NormScaler.h)