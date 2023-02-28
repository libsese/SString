cmake_minimum_required(VERSION 3.14)

project(SString VERSION 0.1.4)

set(CMAKE_CXX_STANDARD 11)

add_library(SString SHARED)
target_include_directories(
        SString
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)
target_sources(SString PRIVATE
        src/algorithm.cpp src/SString.cpp src/SStringBuilder.cpp
        )
add_library(SString-static STATIC)
target_include_directories(
        SString-static
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)
target_sources(SString-static PRIVATE $<TARGET_OBJECTS:SString>)

if (WIN32)
    target_compile_options(SString PRIVATE "/utf-8")
    target_compile_options(SString-static PRIVATE "/utf-8")
    set_target_properties(SString PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif ()

option(SSTRING_BUILD_TEST "build the unit test for sstring lib" OFF)

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

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)
configure_package_config_file(
        ${PROJECT_SOURCE_DIR}/cmake/SStringConfig.cmake.in
        ${PROJECT_BINARY_DIR}/SStringConfig.cmake
        INSTALL_DESTINATION lib/cmake/SString
)

install(
        TARGETS SString SString-static
        EXPORT SStringTargets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        PUBLIC_HEADER DESTINATION include
)

install(
        DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/include/SString"
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
)

install(
        FILES ${PROJECT_BINARY_DIR}/SStringConfig.cmake
        DESTINATION lib/cmake/SString
)

install(
        FILES ${PROJECT_BINARY_DIR}/SStringConfig.cmake
        DESTINATION debug/lib/cmake/SString
)

install(
        EXPORT SStringTargets
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/SString
        NAMESPACE SString::
)