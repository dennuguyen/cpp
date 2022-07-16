#include "directed_weighted_graph.hpp"
#include "gtest/gtest.h"

TEST(empty_graph, directed_weighted_graph) {
    auto g = xtd::directed_weighted_graph<int, std::string>();
    EXPECT_TRUE(g.empty());
}

TEST(empty_initialiser_list, directed_weighted_graph) {
    auto g = xtd::directed_weighted_graph<int, std::string>({});
    EXPECT_TRUE(g.empty());
}

TEST(initialiser_list_with_ints, directed_weighted_graph) {
    auto g = xtd::directed_weighted_graph<int, std::string>({2, 3, 4});
    EXPECT_TRUE(g.is_node(2));
    EXPECT_TRUE(g.is_node(3));
    EXPECT_TRUE(g.is_node(4));
}

TEST(initialiser_list_with_strings, directed_weighted_graph) {
    auto g = xtd::directed_weighted_graph<std::string, std::string>({"abc", "cbd", "ebd"});
    EXPECT_TRUE(g.is_node("abc"));
    EXPECT_TRUE(g.is_node("cbd"));
    EXPECT_TRUE(g.is_node("ebd"));
}

TEST(initialiser_list_with_duplicates, directed_weighted_graph) {
    auto g = xtd::directed_weighted_graph<std::string, std::string>({"abc", "abc", "cbd", "ebd", "ebd"});
    EXPECT_EQ(3, g.size());
    EXPECT_TRUE(g.is_node("abc"));
    EXPECT_TRUE(g.is_node("cbd"));
    EXPECT_TRUE(g.is_node("ebd"));
}

TEST(empty_iterator_constructor, directed_weighted_graph) {
    auto il = std::initializer_list<int>();
    auto g = xtd::directed_weighted_graph<int, std::string>(il.begin(), il.end());
    EXPECT_TRUE(g.empty());
}

TEST(full_range_iterator_constructor, directed_weighted_graph) {
    auto il = {300000000, 5, 1, 912, -4, -335, -1, 0, 81};
    auto g = xtd::directed_weighted_graph<int, double>(il.begin(), il.end());
    EXPECT_TRUE(g.is_node(300000000));
    EXPECT_TRUE(g.is_node(5));
    EXPECT_TRUE(g.is_node(1));
    EXPECT_TRUE(g.is_node(912));
    EXPECT_TRUE(g.is_node(-4));
    EXPECT_TRUE(g.is_node(-335));
    EXPECT_TRUE(g.is_node(-1));
    EXPECT_TRUE(g.is_node(0));
    EXPECT_TRUE(g.is_node(81));
}

TEST(partial_range_iterator_constructor, directed_weighted_graph) {
    auto il = {"first", "second", "third", "fourth", "fifth", "sixth"};
    auto g = xtd::directed_weighted_graph<std::string, double>(il.begin() + 1, il.begin() + 4);
    EXPECT_FALSE(g.is_node("first"));
    EXPECT_TRUE(g.is_node("second"));
    EXPECT_TRUE(g.is_node("third"));
    EXPECT_TRUE(g.is_node("fourth"));
    EXPECT_FALSE(g.is_node("fifth"));
    EXPECT_FALSE(g.is_node("sixth"));
}

TEST(iterator_constructor_with_duplicates, directed_weighted_graph) {
    auto il = {"first", "second", "second", "third", "fourth", "fifth", "sixth"};
    auto g = xtd::directed_weighted_graph<std::string, double>(il.begin() + 1, il.begin() + 4);
    EXPECT_EQ(2, g.size());
    EXPECT_FALSE(g.is_node("first"));
    EXPECT_TRUE(g.is_node("second"));
    EXPECT_TRUE(g.is_node("third"));
    EXPECT_FALSE(g.is_node("fourth"));
    EXPECT_FALSE(g.is_node("fifth"));
    EXPECT_FALSE(g.is_node("sixth"));
}

TEST(copy_empty_graph, directed_weighted_graph) {
    auto g1 = xtd::directed_weighted_graph<int, int>();
    auto g2 = xtd::directed_weighted_graph<int, int>(g1);
    EXPECT_TRUE(g1.empty());
    EXPECT_TRUE(g2.empty());
}

TEST(copy_graph_with_only_nodes, directed_weighted_graph) {
    auto g1 = xtd::directed_weighted_graph<int, int>({3, 4, 6, 8, -4, -1, 0});
    auto g2 = xtd::directed_weighted_graph<int, int>(g1);
    EXPECT_TRUE(g1.is_node(3));
    EXPECT_TRUE(g1.is_node(4));
    EXPECT_TRUE(g1.is_node(6));
    EXPECT_TRUE(g1.is_node(8));
    EXPECT_TRUE(g1.is_node(-4));
    EXPECT_TRUE(g1.is_node(-1));
    EXPECT_TRUE(g1.is_node(0));
    EXPECT_TRUE(g2.is_node(3));
    EXPECT_TRUE(g2.is_node(4));
    EXPECT_TRUE(g2.is_node(6));
    EXPECT_TRUE(g2.is_node(8));
    EXPECT_TRUE(g2.is_node(-4));
    EXPECT_TRUE(g2.is_node(-1));
    EXPECT_TRUE(g2.is_node(0));
}

TEST(copy_graph_with_nodes_and_edges, directed_weighted_graph) {
    auto g1 = xtd::directed_weighted_graph<int, int>({3, 4, 6});
    g1.insert_edge(3, 4, 1);
    ASSERT_TRUE(g1.is_connected(3, 4));
    auto g2 = xtd::directed_weighted_graph(g1);
    EXPECT_TRUE(g1.is_node(3));
    EXPECT_TRUE(g1.is_node(4));
    EXPECT_TRUE(g1.is_node(6));
    EXPECT_TRUE(g1.is_connected(3, 4));
    EXPECT_TRUE(g2.is_node(3));
    EXPECT_TRUE(g2.is_node(4));
    EXPECT_TRUE(g2.is_node(6));
    EXPECT_TRUE(g2.is_connected(3, 4));
}

TEST(move_empty_graph, directed_weighted_graph) {
    auto g1 = xtd::directed_weighted_graph<int, int>();
    auto g2 = xtd::directed_weighted_graph<int, int>(std::move(g1));
    EXPECT_TRUE(g2.empty());
}

TEST(move_graph_with_only_nodes, directed_weighted_graph) {
    auto g1 = xtd::directed_weighted_graph<int, int>({3, 4, 6});
    auto g2 = xtd::directed_weighted_graph<int, int>(std::move(g1));
    EXPECT_TRUE(g2.is_node(3));
    EXPECT_TRUE(g2.is_node(4));
    EXPECT_TRUE(g2.is_node(6));
}

TEST(move_graph_with_nodes_and_edges, directed_weighted_graph) {
    auto g1 = xtd::directed_weighted_graph<int, int>({3, 4, 6});
    g1.insert_edge(3, 4, 1);
    g1.insert_edge(3, 6, 1);
    auto g2 = xtd::directed_weighted_graph<int, int>(std::move(g1));
    EXPECT_TRUE(g2.is_node(3));
    EXPECT_TRUE(g2.is_node(4));
    EXPECT_TRUE(g2.is_node(6));
    EXPECT_TRUE(g2.is_connected(3, 4));
    EXPECT_TRUE(g2.is_connected(3, 6));
}

TEST(copy_assign_empty_graph, directed_weighted_graph) {
    auto g1 = xtd::directed_weighted_graph<int, int>();
    auto g2 = xtd::directed_weighted_graph<int, int>({0, -1, -2});
    g2 = g1;
    EXPECT_TRUE(g1.empty());
    EXPECT_TRUE(g2.empty());
}

TEST(copy_assign_graph_with_only_nodes, directed_weighted_graph) {
    auto g1 = xtd::directed_weighted_graph<int, int>({3, 4, 6});
    auto g2 = xtd::directed_weighted_graph<int, int>({0, -1, -2});
    g2 = g1;
    EXPECT_TRUE(g1.is_node(3));
    EXPECT_TRUE(g1.is_node(4));
    EXPECT_TRUE(g1.is_node(6));
    EXPECT_TRUE(g2.is_node(3));
    EXPECT_TRUE(g2.is_node(4));
    EXPECT_TRUE(g2.is_node(6));
}

TEST(copy_assign_graph_with_nodes_and_edges, directed_weighted_graph) {
    auto g1 = xtd::directed_weighted_graph<int, int>({3, 4, 6});
    g1.insert_edge(3, 4, 1);
    g1.insert_edge(3, 6, 1);
    ASSERT_TRUE(g1.is_connected(3, 4));
    ASSERT_TRUE(g1.is_connected(3, 6));
    auto g2 = xtd::directed_weighted_graph<int, int>({0, -1, -2});
    g2 = g1;
    EXPECT_TRUE(g1.is_node(3));
    EXPECT_TRUE(g1.is_node(4));
    EXPECT_TRUE(g1.is_node(6));
    EXPECT_TRUE(g1.is_connected(3, 4));
    EXPECT_TRUE(g1.is_connected(3, 6));
    EXPECT_TRUE(g2.is_node(3));
    EXPECT_TRUE(g2.is_node(4));
    EXPECT_TRUE(g2.is_node(6));
    EXPECT_TRUE(g2.is_connected(3, 4));
    EXPECT_TRUE(g2.is_connected(3, 6));
}

TEST(move_assign_empty_graph, directed_weighted_graph) {
    auto g1 = xtd::directed_weighted_graph<int, int>();
    auto g2 = xtd::directed_weighted_graph<int, int>({0, -1, -2});
    g2 = std::move(g1);
    EXPECT_TRUE(g2.empty());
}

TEST(move_assign_graph_with_only_nodes, directed_weighted_graph) {
    auto g1 = xtd::directed_weighted_graph<int, int>({0, -1, -2});
    auto g2 = xtd::directed_weighted_graph<int, int>();
    g2 = std::move(g1);
    EXPECT_TRUE(g2.is_node(0));
    EXPECT_TRUE(g2.is_node(-1));
    EXPECT_TRUE(g2.is_node(-2));
}

TEST(move_assign_graph_with_nodes_and_edges, directed_weighted_graph) {
    auto g1 = xtd::directed_weighted_graph<int, int>({3, 4, 6});
    g1.insert_edge(3, 4, 1);
    g1.insert_edge(3, 6, 1);
    ASSERT_TRUE(g1.is_connected(3, 4));
    ASSERT_TRUE(g1.is_connected(3, 6));
    auto g2 = xtd::directed_weighted_graph<int, int>({0, -1, -2});
    g2 = std::move(g1);
    EXPECT_TRUE(g2.is_node(3));
    EXPECT_TRUE(g2.is_node(4));
    EXPECT_TRUE(g2.is_node(6));
    EXPECT_TRUE(g2.is_connected(3, 4));
    EXPECT_TRUE(g2.is_connected(3, 6));
}

TEST(clear_graph, directed_weighted_graph) {
    auto g = xtd::directed_weighted_graph<std::string, int>({"1", "2", "3"});
    ASSERT_FALSE(g.empty());
    g.clear();
    EXPECT_TRUE(g.empty());
}

TEST(empty_size, directed_weighted_graph) {
    auto g = xtd::directed_weighted_graph<std::string, int>();
    EXPECT_EQ(0, g.size());
}

TEST(nonempty_size, directed_weighted_graph) {
    auto g = xtd::directed_weighted_graph<std::string, int>({"1", "2"});
    EXPECT_EQ(2, g.size());
    g.insert_edge("1", "2", 1);  // Should have no effect on size.
    EXPECT_EQ(2, g.size());
}

TEST(is_empty, directed_weighted_graph) {
    auto g = xtd::directed_weighted_graph<std::string, int>();
    EXPECT_TRUE(g.empty());
}

TEST(is_not_empty, directed_weighted_graph) {
    auto g = xtd::directed_weighted_graph<std::string, int>({"1", "2"});
    EXPECT_FALSE(g.empty());
}