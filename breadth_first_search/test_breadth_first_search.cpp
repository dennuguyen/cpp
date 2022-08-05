#include "breadth_first_search.hpp"
#include "directed_weighted_graph.hpp"
#include "gtest/gtest.h"

TEST(breadth_first_search, empty_graph) {
    auto g = xtd::directed_weighted_graph<int, std::string>();
    EXPECT_THROW(xtd::breadth_first_search(g, 2, 3), std::runtime_error);
}

TEST(breadth_first_search, single_shortest_path) {
    auto g = xtd::directed_weighted_graph<int, std::string>({1, 2, 3});
    ASSERT_TRUE(g.insert_edge(1, 2, "a"));
    ASSERT_TRUE(g.insert_edge(2, 3, "a"));
    auto v = xtd::breadth_first_search(g, 1, 3);
    EXPECT_EQ(1, v.size());
    EXPECT_EQ(std::vector<int>({1, 2, 3}), v.at(0));
}

TEST(breadth_first_search, multiple_shortest_paths) {
    auto g = xtd::directed_weighted_graph<int, std::string>({1, 2, 3});
    ASSERT_TRUE(g.insert_edge(1, 2, "a"));
    ASSERT_TRUE(g.insert_edge(1, 2, "b"));
    ASSERT_TRUE(g.insert_edge(2, 3, "a"));
    auto v = xtd::breadth_first_search(g, 1, 3);
    EXPECT_EQ(2, v.size());
    EXPECT_EQ(std::vector<int>({1, 2, 3}), v.at(0));
    EXPECT_EQ(std::vector<int>({1, 2, 3}), v.at(1));
}

TEST(breadth_first_search, multiple_shortest_and_nonshortest_paths) {
    auto g = xtd::directed_weighted_graph<int, std::string>({1, 2, 3, 4});
    ASSERT_TRUE(g.insert_edge(1, 2, "a"));
    ASSERT_TRUE(g.insert_edge(1, 2, "b"));
    ASSERT_TRUE(g.insert_edge(2, 3, "a"));
    ASSERT_TRUE(g.insert_edge(3, 4, "a"));
    ASSERT_TRUE(g.insert_edge(2, 4, "a"));
    auto v = xtd::breadth_first_search(g, 1, 4);
    EXPECT_EQ(4, v.size());
    EXPECT_EQ(std::vector<int>({1, 2, 4}), v.at(0));
    EXPECT_EQ(std::vector<int>({1, 2, 4}), v.at(1));
    EXPECT_EQ(std::vector<int>({1, 2, 3, 4}), v.at(2));
    EXPECT_EQ(std::vector<int>({1, 2, 3, 4}), v.at(3));
}

TEST(breadth_first_search, forest) {
    auto g = xtd::directed_weighted_graph<int, std::string>({1, 2, 3, 4});
    ASSERT_TRUE(g.insert_edge(1, 2, "a"));
    ASSERT_TRUE(g.insert_edge(3, 4, "a"));
    auto v = xtd::breadth_first_search(g, 1, 4);
    EXPECT_TRUE(v.empty());
}