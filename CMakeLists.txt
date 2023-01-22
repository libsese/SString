cmake_minimum_required(VERSION 3.14)

project(SString VERSION 0.1.3)

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
    set_target_properties(SString PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif ()

option(SSTRING_BUILD_TEST "build the unit test for uuid lib" OFF)

if (SSTRING_BUILD_TEST)
    find_package(GTest CONFIG REQUIRED)
    enable_testing()

    add_executable(TestAlgol test/TestAlgol.cpp)
    target_link_libraries(TestAlgol SString GTest::gtest GTest::gtest_main)
    add_test(NAME TestAlgol COMMAND TestAlgol)
    if (WIN32)
        target_compile_options(TestAlgol PRIVATE /utf-8)
    endif ()

    add_executable(TestSString test/TestSString.cpp)
    target_link_libraries(TestSString SString GTest::gtest GTest::gtest_main)
    add_test(NAME TestSString COMMAND TestSString)
    if (WIN32)
        target_compile_options(TestSString PRIVATE /utf-8)
    endif ()

    add_executable(TestSStringBuilder test/TestSStringBuilder.cpp)
    target_link_libraries(TestSStringBuilder SString GTest::gtest GTest::gtest_main)
    add_test(NAME TestSStringBuilder COMMAND TestSStringBuilder)
    if (WIN32)
        target_compile_options(TestSStringBuilder PRIVATE /utf-8)
    endif ()
endif ()