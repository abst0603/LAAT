cmake_minimum_required(VERSION 3.10)

project(LAAT VERSION 0.0)

include_directories(src)

file(GLOB SOURCES "src/*.cpp" "src/datapoint/*.cpp")

add_executable(LAAT ${SOURCES})

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "-O2 -Wall")
