find_package(GTest REQUIRED)
include(GoogleTest)

include_directories(${GTEST_INCLUDE_DIRS})

set(TESTS
    test_contains
    test_transform_if
    test_multikey_map
    test_reverse_container
    test_sliding_window
)

foreach(TEST ${TESTS})
    add_executable(${TEST} ${TEST}.cpp)
    target_link_libraries(${TEST} pthread gtest gtest_main)
    add_test(
        NAME ${TEST}
        COMMAND ${TEST}
    )
endforeach()
