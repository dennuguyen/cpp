find_package(GTest REQUIRED)
include(GoogleTest)

include_directories(${GTEST_INCLUDE_DIRS})

add_executable(contains_test test_contains.cpp)
target_link_libraries(contains_test pthread gtest gtest_main)

add_test(
    NAME contains_test
    COMMAND contains_test
)