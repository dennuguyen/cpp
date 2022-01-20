#include "gtest/gtest.h"
#include "reverse_container.hpp"

TEST(random_access_iterator, reverse_container) {
    std::vector<int> nums = {0, 1, 2, 3, 4, 5, 6};
    std::vector<int> expected = {6, 5, 4, 3, 2, 1, 0};
    std::vector<int> actual = xtl::reverse_container(nums);
    EXPECT_EQ(expected, actual);
    EXPECT_NE(actual, nums);
}

TEST(empty, reverse_container) {
    std::vector<int> nums = {};
    std::vector<int> expected = {};
    std::vector<int> actual = xtl::reverse_container(nums);
    EXPECT_EQ(expected, actual);
}
