find_package(GTest REQUIRED)
include(GoogleTest)

include_directories(${GTEST_INCLUDE_DIRS})

add_executable(multikey_map_test test_constructor.cpp)
target_link_libraries(multikey_map_test pthread gtest gtest_main)

add_test(
    NAME multikey_map_test
    COMMAND multikey_map_test
)