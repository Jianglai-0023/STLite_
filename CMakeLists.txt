cmake_minimum_required(VERSION 3.20)
project(STLite_ACM_master)

set(CMAKE_CXX_STANDARD 14)

include_directories(priority_queue/src)

add_executable(STLite_ACM_master
        map/data/toy_traits_test/test.cpp
        map/exceptions.hpp
        map/data/toy_traits_test/map.hpp
        map/utility.hpp)
