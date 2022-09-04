cmake_minimum_required(VERSION 3.14)
project(SString)

set(CMAKE_CXX_STANDARD 11)

include_directories(include)

if (CMAKE_SYSTEM_NAME MATCHES "Windows")
    add_compile_options("/utf-8")
endif ()

add_library(SString src/algorithm.cpp src/SString.cpp)

enable_testing()
add_executable(TestSString test/TestSString.cpp)
target_link_libraries(TestSString SString)
add_test(NAME TestSString COMMAND TestSString)
add_executable(TestAlgol test/TestAlgol.cpp)
target_link_libraries(TestAlgol SString)
add_test(NAME TestAlgol COMMAND TestAlgol)