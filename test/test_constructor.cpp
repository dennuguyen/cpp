#include "gtest/gtest.h"
#include "multikey_map.hpp"

TEST(defaultConstructor, oneKey) {
    multikey_map<int, std::string> map;
    map[3] = "hello world";
    EXPECT_EQ("hello world", map[3]);
}