cmake_minimum_required(VERSION 3.10)
project(cache_aware_oblivious_merge_sort LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_definitions(-DNOMINMAX)
add_executable(merge main.cpp)