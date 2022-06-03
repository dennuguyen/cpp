#include "adjacent_all_of.hpp"
#include "gtest/gtest.h"

TEST(handles_ascending, ascending) {
    std::vector<int> nums = {1, 2, 3, 4};
    auto result = xtd::adjacent_all_of(
        nums.begin(), nums.end(), [](auto const& l, auto const& r) { return l < r; });

    EXPECT_EQ(result, true);
}

TEST(handles_descending, ascending) {
    std::vector<int> nums = {4, 3, 2, 1};
    auto result = xtd::adjacent_all_of(
        nums.begin(), nums.end(), [](auto const& l, auto const& r) { return l < r; });

    EXPECT_EQ(result, false);
}

TEST(handles_equal, all_equal) {
    std::vector<int> nums = {1, 1, 1, 1};
    auto result = xtd::adjacent_all_of(
        nums.begin(), nums.end(), [](auto const& l, auto const& r) { return l == r; });

    EXPECT_EQ(result, true);
}

TEST(handles_inequal, all_equal) {
    std::vector<int> nums = {1, 2, 1, 1};
    auto result = xtd::adjacent_all_of(
        nums.begin(), nums.end(), [](auto const& l, auto const& r) { return l == r; });

    EXPECT_EQ(result, false);
}

TEST(handles_partially_equal, vectors_of_pairs) {
    auto operator==(std::pair<int, int> lhs, std::pair<int, int> rhs) noexcept->bool {
        return lhs.second == rhs.first;
    };

    std::vector<std::pair<int, int>> nums = {{1, 3}, {3, 4}, {4, 18}, {18, 3}};
    auto result = xtd::adjacent_all_of(
        nums.begin(), nums.end(), [](auto const& l, auto const& r) { return l == r; });

    EXPECT_EQ(result, false);
}

TEST(handles_partially_inequal, vectors_of_pairs) {
    auto operator==(std::pair<int, int> lhs, std::pair<int, int> rhs) noexcept->bool {
        return lhs.second == rhs.first;
    };

    std::vector<std::pair<int, int>> nums = {{1, 2}, {1, 1}};
    auto result = xtd::adjacent_all_of(
        nums.begin(), nums.end(), [](auto const& l, auto const& r) { return l == r; });

    EXPECT_EQ(result, false);
}