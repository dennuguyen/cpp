#include <algorithm>
#include <iterator>

#include "gtest/gtest.h"
#include "sliding_window.hpp"

TEST(nums_addition_0_window, sliding_window) {
    std::vector<int> nums = {1, 2, 3, 4};
    std::vector<int> actual;
    auto end = xtl::sliding_window(
        nums.begin(), nums.end(), std::back_inserter(actual), 0, [](auto const& l, auto const& r) { return *r > *l; }, [](auto const& l, auto const& r) { return *l + *r; });

    std::vector<int> expected = {3, 5, 7, 4, 6, 5};
    EXPECT_EQ(expected, actual);
}

TEST(nums_addition_1_window, sliding_window) {
    std::vector<int> nums = {1, 2, 3, 4};
    std::vector<int> actual;
    auto end = xtl::sliding_window(
        nums.begin(), nums.end(), std::back_inserter(actual), 1, [](auto const& l, auto const& r) { return *r > *l; }, [](auto const& l, auto const& r) { return *l + *r; });

    std::vector<int> expected = {3, 5, 7, 4, 6, 5};
    EXPECT_EQ(expected, actual);
}

TEST(nums_addition_2_window, sliding_window) {
    std::vector<int> nums = {1, 2, 3, 4};
    std::vector<int> actual;
    auto end = xtl::sliding_window(
        nums.begin(), nums.end(), std::back_inserter(actual), 2, [](auto const& l, auto const& r) { return *r > *l; }, [](auto const& l, auto const& r) { return *l + *r; });

    std::vector<int> expected = {4, 6, 5};
    EXPECT_EQ(expected, actual);
}

TEST(empty_string, sliding_window) {
    std::string str = "";
    std::string expected = {""};
    std::string actual;
    xtl::sliding_window(
        str.begin(), str.end(), std::back_inserter(actual), 1, [](auto const& l, auto const& r) { return true; }, [](auto const& l, auto const& r) { return *l; });
    EXPECT_EQ(expected, actual);
}

TEST(nested_palindromes, sliding_window) {
    auto to_string = [](std::string::iterator const& begin, std::string::iterator const& end) -> std::string {
        return std::string(begin, end);
    };

    auto is_palindrome = [](std::string::iterator const& begin, std::string::iterator const& end) -> bool {
        auto s = std::string(begin, end);
        auto rs = s;
        std::reverse(rs.begin(), rs.end());
        return s == rs;
    };

    std::string str = "heabalopo";
    std::vector<std::string> expected = {"aba", "opo"};
    std::vector<std::string> actual;
    xtl::sliding_window(str.begin(), str.end(), std::back_inserter(actual), 3, is_palindrome, to_string);
    EXPECT_EQ(expected, actual);
}