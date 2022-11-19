cmake_minimum_required(VERSION 3.14)
project(SString)

set(CMAKE_CXX_STANDARD 11)

add_library(SString SHARED)
target_include_directories(SString PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)
target_sources(SString PRIVATE
        src/algorithm.cpp src/SString.cpp src/SStringBuilder.cpp
)
add_library(SString-static)
target_include_directories(SString-static PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)
target_sources(SString-static PRIVATE $<TARGET_OBJECTS:SString>)

if (WIN32)
    target_compile_options(SString PRIVATE "/utf-8")
    target_compile_options(SString-static PRIVATE "/utf-8")
endif ()