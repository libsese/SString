cmake_minimum_required(VERSION 3.14)
project(SString)

set(CMAKE_CXX_STANDARD 11)

include_directories(include)

add_library(SString src/SString.cpp)
