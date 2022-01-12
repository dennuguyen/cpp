#include "MultiMap.hpp"
#include "gtest/gtest.h"

TEST(MultiMapTest, oneKey) {
    MultiMap<int, std::string> map;
    map[3] = "hello world";
    EXPECT_EQ("hello world", map[3]);
}