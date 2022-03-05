#include <algorithm>

#include "gtest/gtest.h"
#include "sliding_window.hpp"

TEST(number_slides, sliding_window) {
    std::vector<int> nums = {1, 2, 3, 4};
    std::vector<int> actual;
    actual.reserve(100);
    auto end = xtl::sliding_window(
        nums.begin(), nums.end(), actual.begin(), 1, [](auto const& l, auto const& r) { return *r > *l; }, [](auto const& l, auto const& r) { return *l + *r; });
    actual.resize(end - actual.begin());

    std::vector<int> expected = {3, 5, 7, 4, 6, 5};
    EXPECT_EQ(expected, actual);
}

// TEST(nested_palindromes, sliding_window) {
//     auto to_string = [](std::string::iterator const& begin, std::string::iterator const& end) -> std::string {
//         return std::string(begin, end);
//     };

//     auto is_palindrome = [](std::string::iterator const& begin, std::string::iterator const& end) -> bool {
//         auto s = std::string(begin, end);
//         auto rs = s;
//         std::reverse(rs.begin(), rs.end());
//         return s == rs;
//     };

//     std::string str = "heabalopo";
//     std::vector<std::string> expected = {"aba", "opo"};
//     std::vector<std::string> actual;
//     actual.reserve(100);
//     xtl::sliding_window(str.begin(), str.end(), actual.begin(), 1, is_palindrome, to_string);
//     EXPECT_EQ(expected, actual);
// }
