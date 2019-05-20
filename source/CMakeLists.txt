cmake_minimum_required(VERSION 3.13)
project(BenchmarkFunctions2)

set(CMAKE_CXX_STANDARD 14)

add_executable(BenchmarkFunctions2 main.cpp BenchmarkFunctions.cpp BenchmarkFunctions.h utilities.cpp utilities.h DataStructs.h ProcessFunctions.cpp ProcessFunctions.h SearchAlgorithms.cpp SearchAlgorithms.h FilenameConstants.h)