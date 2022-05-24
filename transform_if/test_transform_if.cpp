#include "gtest/gtest.h"
#include "transform_if.hpp"

TEST(always_doubles, transform_if) {
    std::vector<int> nums = {0, 1, 2, 3, 4, 5, 6};
    std::vector<int> expected = {0, 2, 4, 6, 8, 10, 12};
    std::vector<int> actual(nums.size());
    xtd::transform_if(
        nums.begin(), nums.end(), actual.begin(), [](auto const& i) { return true; }, [](auto const& i) { return i * 2; });
    EXPECT_EQ(expected, actual);
}

TEST(never_doubles, transform_if) {
    std::vector<int> nums = {0, 1, 2, 3, 4, 5, 6};
    std::vector<int> expected = {0, 0, 0, 0, 0, 0, 0};
    std::vector<int> actual(nums.size());
    xtd::transform_if(
        nums.begin(), nums.end(), actual.begin(), [](auto const& i) { return false; }, [](auto const& i) { return i * 2; });
    EXPECT_EQ(expected, actual);
}

TEST(doubles_if_odd, transform_if) {
    std::vector<int> nums = {0, 1, 2, 3, 4, 5, 6};
    std::vector<int> expected = {0, 2, 0, 6, 0, 10, 0};
    std::vector<int> actual(nums.size());
    xtd::transform_if(
        nums.begin(), nums.end(), actual.begin(), [](auto const& i) { return i % 2; }, [](auto const& i) { return i * 2; });
    EXPECT_EQ(expected, actual);
}