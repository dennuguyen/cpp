#include "gtest/gtest.h"
#include "multikey_map.hpp"

TEST(default_constructor, test_default_constructor) {
    multikey_map<int, std::string> map;
    EXPECT_EQ(0, map.size());
}

TEST(empty_initialiser_list, test_initialiser_list) {
    multikey_map<int, std::string> map = {};
    EXPECT_EQ(0, map.size());
}

TEST(single_nested_initialiser_list, test_initialiser_list) {
    multikey_map<int, std::string> map = {
        {0, "hello"},
        {1, "world"},
    };
    EXPECT_EQ(2, map.size());
}

TEST(double_nested_initialiser_list, test_initialiser_list) {
    multikey_map<int, int, std::string> map = {
        {
            0,
            {
                {0, "hello"},
                {1, "world"},
            },
        },
        {
            1,
            {
                {0, "bye"},
                {1, "everyone"},
            },
        },
    };
    EXPECT_EQ(0, map.size());
}

TEST(triple_nested_initialiser_list, test_initialiser_list) {
    multikey_map<int, std::string> map;
    EXPECT_EQ(0, map.size());
}

TEST(copy_constructor, test_constructor) {
    multikey_map<int, char, int, std::string> map;
    map[3]['e'][0] = "hello world";
    EXPECT_EQ("hello world", map[3]['e'][0]);
}