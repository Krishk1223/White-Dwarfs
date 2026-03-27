cmake_minimum_required(VERSION 3.20...4.2.3)
project(white_dwarfs VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#find raylib
find_package(raylib REQUIRED)

add_executable(white_dwarfs
    src/main.cpp
    src/eqofstate.cpp
    src/solver.cpp
)

target_link_libraries(white_dwarfs raylib)

