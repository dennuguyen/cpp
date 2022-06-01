cmake_minimum_required(VERSION 3.2.2)
project(competitive_programming_handbook VERSION 0.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(CTest)
enable_testing()

macro(build_test_suite TEST)
    find_package(GTest REQUIRED)

    include(GoogleTest)

    include_directories(${GTEST_INCLUDE_DIRS})

    add_executable(${TEST} ${TEST}.cpp)

    target_link_libraries(${TEST} pthread gtest gtest_main)

    add_test(NAME ${TEST} COMMAND ${TEST})
endmacro()

add_subdirectory(adjacent_all_of)
add_subdirectory(sliding_window)

