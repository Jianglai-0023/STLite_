cmake_minimum_required(VERSION 3.20)
project(STLite_ACM_master)

set(CMAKE_CXX_STANDARD 14)

include_directories(priority_queue/src)

add_executable(STLite_ACM_master
        map/data/five/code.cpp
        map/exceptions.hpp
        map/data/five/map.hpp
        map/utility.hpp)
