cmake_minimum_required(VERSION 3.10)

project(string2map)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_library(string2map STATIC src/string2map.h src/string2map.cpp)