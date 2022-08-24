cmake_minimum_required(VERSION 3.14)
project(SString)

set(CMAKE_CXX_STANDARD 11)

include_directories(include)

if (CMAKE_SYSTEM_NAME MATCHES "Windows")
    add_compile_options("/utf-8")
endif ()

add_library(SString src/SString.cpp)

enable_testing()
add_executable(TestSString test/TestSString.cpp)
target_link_libraries(TestSString SString)
add_test(NAME TestSString COMMAND TestSString)