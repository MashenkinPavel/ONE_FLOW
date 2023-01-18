cmake_minimum_required(VERSION 3.14)
project(ONE_FLOW)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(ONE_FLOW
        main.cpp
        mesh.cpp
        mesh.h node.cpp node.h face.cpp face.h cell.cpp cell.h)
