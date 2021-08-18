cmake_minimum_required(VERSION 3.10)

project(LAAT VERSION 0.0)

# Threads
find_package(Threads REQUIRED)
# Eigen
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/ext/eigen")
# OpenMP
find_package(OpenMP REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "-O2 -Wall -fPIC")

add_library(laatlib
    src/lib/LAAT/antsearch.cpp
    src/lib/LAAT/evaporatepheromone.cpp
    src/lib/LAAT/groupdata.cpp
    src/lib/LAAT/initializeants.cpp
    src/lib/LAAT/LAAT.cpp
    src/lib/LAAT/lostants.cpp
    src/lib/LAAT/preprocess.cpp
    src/lib/LAAT/printwarning.cpp
    src/lib/LAAT/progressbar.cpp
    src/lib/LAAT/rangesearch.cpp

    src/lib/MBMS/denoise.cpp
    src/lib/MBMS/dimensionalityapproximation.cpp
    src/lib/MBMS/kernelfunmean.cpp
    src/lib/MBMS/localpca.cpp
    src/lib/MBMS/MBMS.cpp
    src/lib/MBMS/rwcsv.cpp
)

# add include directories to use when compiling the target
target_include_directories(laatlib PRIVATE ext)
target_include_directories(laatlib PUBLIC src/lib)

target_link_libraries(laatlib Eigen3::Eigen OpenMP::OpenMP_CXX)

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/ext/pybind11")
pybind11_add_module(LAAT src/python_bindings/bindings.cpp)
target_link_libraries(LAAT PRIVATE laatlib)
