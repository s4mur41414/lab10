cmake_minimum_required(VERSION 3.28)
project(10lab)

set(CMAKE_CXX_STANDARD 23)

add_executable(10lab main.cpp
        lib/TTaskScheduler.h
        lib/TTaskScheduler.cpp
        lib/Graph.h)

enable_testing()
add_subdirectory(tests)
