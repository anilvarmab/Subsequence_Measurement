cmake_minimum_required(VERSION 3.10)
project(subsequence)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

option(USE_CHRONO "Use std:chrono for timings")
if(USE_CHRONO)
    add_definitions(-DUSE_CHRONO)
endif(USE_CHRONO)

configure_file(${PROJECT_SOURCE_DIR}/measure.py
               ${PROJECT_BINARY_DIR}/measure.py COPYONLY)

add_executable(subsequence subsequence.cpp timer.cpp timer.h)
