#include "gtest/gtest.h"
#include "tokenise.hpp"

TEST(space_delimiter, tokenise) {
    auto str = "a b c";
    auto vec = xtd::tokenise(str, " ");
    EXPECT_EQ(3, vec.size());
    EXPECT_EQ("a", vec[0]);
    EXPECT_EQ("b", vec[1]);
    EXPECT_EQ("c", vec[2]);
}

TEST(new_line_delimtier, tokenise) {
    auto str = "a\nb\nc\n";
    auto vec = xtd::tokenise(str, "\n");
    EXPECT_EQ(3, vec.size());
    EXPECT_EQ("a", vec[0]);
    EXPECT_EQ("b", vec[1]);
    EXPECT_EQ("c", vec[2]);
}

TEST(string_delimtier, tokenise) {
    auto str = "abccabacbccba";
    auto vec = xtd::tokenise(str, "cc");
    EXPECT_EQ(3, vec.size());
    EXPECT_EQ("ab", vec[0]);
    EXPECT_EQ("abacb", vec[1]);
    EXPECT_EQ("ba", vec[2]);
}
