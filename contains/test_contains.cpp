#include "gtest/gtest.h"
#include "contains.hpp"

TEST(begins_with_contiguous_subset, contains) {
    std::vector<int> alpha = {0, 3, 2, 6};
    std::vector<int> test = {0, 3, 2};
    EXPECT_TRUE(xtd::contains(alpha, test));
}

TEST(ends_with_contiguous_subset, contains) {
    std::vector<int> alpha = {0, 3, 2, 6};
    std::vector<int> test = {3, 2, 6};
    EXPECT_TRUE(xtd::contains(alpha, test));
}

TEST(duplicated_value_in_alphabet, contains) {
    std::string alpha = "abbbbc";
    std::string word = "bb";
    EXPECT_TRUE(xtd::contains(alpha, word));
}

TEST(identical_sets, contains) {
    std::vector<int> alpha = {0, 3, 2, 6};
    std::vector<int> test = {0, 3, 2, 6};
    EXPECT_TRUE(xtd::contains(alpha, test));
}

TEST(empty_value, contains) {
    std::vector<char> alpha = {'a', 'b', 'c'};
    std::vector<char> word = {};
    EXPECT_TRUE(xtd::contains(alpha, word));
}

TEST(noncontiguous_subset, contains) {
    std::vector<int> alpha = {0, 3, 2, 6};
    std::vector<int> test = {0, 2};
    EXPECT_FALSE(xtd::contains(alpha, test));
}

TEST(consecutive_but_not_contiguous, contains) {
    std::vector<int> alpha = {0, 3, 2, 6};
    std::vector<int> test = {2, 3, 0};
    EXPECT_FALSE(xtd::contains(alpha, test));
}

TEST(single_char_alphabet, contains) {
    std::string alpha = "a";
    std::string word = "aaaaaaa";
    EXPECT_FALSE(xtd::contains(alpha, word));
}

TEST(single_value_duplicated, contains) {
    std::string alpha = "abcdefghijklmnopqrstuvwxyz";
    std::string word = "hhhhh";
    EXPECT_FALSE(xtd::contains(alpha, word));
}

TEST(multiple_values_duplicated, contains) {
    std::string alpha = "abcdefghijklmnopqrstuvwxyz";
    std::string word = "abcabcabcabc";
    EXPECT_FALSE(xtd::contains(alpha, word));
}

TEST(single_missing_value, contains) {
    std::string alpha = "orms";
    std::string word = "worms";
    EXPECT_FALSE(xtd::contains(alpha, word));
}

TEST(empty_alphabet, contains) {
    std::vector<int> alpha = {};
    std::vector<int> word = {0, 2, 3, 5, 6, 71, 1};
    EXPECT_FALSE(xtd::contains(alpha, word));
}

TEST(both_empty_sets, contains) {
    std::vector<int> alpha = {};
    std::vector<int> test = {};
    EXPECT_FALSE(xtd::contains(alpha, test));
}

TEST(sentence1, contains) {
    std::string alpha = "I am writing a contains template function";
    std::string word1 = "I";
    std::string word2 = "am";
    std::string word3 = " contains template";
    std::string word4 = "function.";
    EXPECT_TRUE(xtd::contains(alpha, word1));
    EXPECT_TRUE(xtd::contains(alpha, word2));
    EXPECT_TRUE(xtd::contains(alpha, word3));
    EXPECT_FALSE(xtd::contains(alpha, word4));
}
