cmake_minimum_required(VERSION 3.20)
project(STLite_ACM_master)

set(CMAKE_CXX_STANDARD 14)

include_directories(priority_queue/src)

add_executable(STLite_ACM_master
        linked_hashmap/data/testone/1.cpp
        linked_hashmap/exceptions.hpp
        linked_hashmap/data/testone/linked_hashmap.hpp
        linked_hashmap/utility.hpp)
