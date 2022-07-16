cmake_minimum_required(VERSION 3.2.2)
project(competitive_programming_handbook VERSION 0.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(CTest)
enable_testing()

macro(build_gtest_suite TEST)
    find_package(GTest REQUIRED)

    include(GoogleTest)

    include_directories(${GTEST_INCLUDE_DIRS})

    add_executable(${TEST} ${TEST}.cpp)

    target_link_libraries(${TEST} pthread gtest gtest_main)

    gtest_discover_tests(${TEST})
endmacro()

add_subdirectory(adjacent_all_of)
add_subdirectory(binary_search_index)
add_subdirectory(contains)
add_subdirectory(directed_weighted_graph)
add_subdirectory(dynamic_programming)
add_subdirectory(maximum_disjoint_set)
add_subdirectory(multikey_map)
add_subdirectory(nested_initializer)
add_subdirectory(reverse_container)
add_subdirectory(sliding_window)
add_subdirectory(tokenise)
add_subdirectory(transform_if)
