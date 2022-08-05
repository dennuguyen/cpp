#include "gtest/gtest.h"
#include "maximum_disjoint_set.hpp"

TEST(maximum_disjoint_set, non_overlapping_string) {
    auto str = std::string("abcdefghijk");
    auto input = std::vector<std::pair<std::string::iterator, std::string::iterator>>({{str.begin(), str.begin() + 1}, {str.begin() + 3, str.begin() + 4}});
    auto output = xtd::maximum_disjoint_set(input);
    EXPECT_EQ(2, output.size());
}

TEST(maximum_disjoint_set, overlapping_string) {
    auto str = std::string("abcdefghijk");
    auto input = std::vector<std::pair<std::string::iterator, std::string::iterator>>({{str.begin(), str.begin() + 3}, {str.begin() + 2, str.begin() + 4}, {str.begin() + 4, str.begin() + 6}});
    auto output = xtd::maximum_disjoint_set(input);
    EXPECT_EQ(1, output.size());
}
