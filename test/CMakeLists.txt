find_package(GTest REQUIRED)
include(GoogleTest)

include_directories(${GTEST_INCLUDE_DIRS})

add_executable(MultiMapTest MultiMapTest.cpp)
target_link_libraries(MultiMapTest pthread gtest gtest_main)

add_test(
    NAME MultiMapTest
    COMMAND MultiMapTest
)