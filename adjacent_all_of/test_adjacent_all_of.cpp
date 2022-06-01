#include "adjacent_all_of.hpp"
#include "gtest/gtest.h"

TEST(ascending, adjacent_all_of) {
    std::vector<int> nums = {1, 2, 3, 4};
    auto result = xtd::adjacent_all_of(
        nums.begin(), nums.end(), [](auto const& l, auto const& r) { return l < r; });

    EXPECT_EQ(result, true);
}
