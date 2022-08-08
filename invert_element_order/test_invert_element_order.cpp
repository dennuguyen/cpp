#include "gtest/gtest.h"
#include "invert_element_order.hpp"

TEST(invert_element_order, numerical) {
    auto v = std::vector<int>({1, 4, 3, 2});
    xtd::invert_element_order(v.begin(), v.end());
    auto w = std::vector<int>({4, 1, 2, 3});
    EXPECT_EQ(w, v);
}

TEST(invert_element_order, alpha) {
    auto v = std::vector<std::string>({"abc", "cba", "bde"});
    xtd::invert_element_order(v.begin(), v.end());
    auto w = std::vector<std::string>({"cba", "abc", "bde"});
    EXPECT_EQ(w, v);
}