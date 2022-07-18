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
    auto g =
        xtd::directed_weighted_graph<std::string, std::string>({"abc", "abc", "cbd", "ebd", "ebd"});
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

TEST(empty_nodes, directed_weighted_graph) {
    auto g = xtd::directed_weighted_graph<std::string, int>();
    auto v = std::vector<std::string>();
    EXPECT_EQ(g.nodes(), v);
}

TEST(sorted_nodes, directed_weighted_graph) {
    auto g = xtd::directed_weighted_graph<std::string, int>({"back", "apple", "bear", "arch", ""});
    auto v = std::vector<std::string>({"", "apple", "arch", "back", "bear"});
    EXPECT_EQ(g.nodes(), v);
}

TEST(is_node, directed_weighted_graph) {
    auto g = xtd::directed_weighted_graph<int, bool>({3, 4, 6, 8, -4, -1, 0, 23, 53, -59});
    EXPECT_TRUE(g.is_node(3));
    EXPECT_TRUE(g.is_node(4));
    EXPECT_TRUE(g.is_node(6));
    EXPECT_TRUE(g.is_node(8));
    EXPECT_TRUE(g.is_node(-4));
    EXPECT_TRUE(g.is_node(-1));
    EXPECT_TRUE(g.is_node(0));
    EXPECT_TRUE(g.is_node(23));
    EXPECT_TRUE(g.is_node(53));
    EXPECT_TRUE(g.is_node(-59));
}

TEST(is_not_node, directed_weighted_graph) {
    auto g = xtd::directed_weighted_graph<int, bool>({3, 4, 6, 8, -4, -1, 0, 23, 53, -59});
    EXPECT_FALSE(g.is_node(1));
    EXPECT_FALSE(g.is_node(2));
    EXPECT_FALSE(g.is_node(5));
    EXPECT_FALSE(g.is_node(7));
    EXPECT_FALSE(g.is_node(-2));
    EXPECT_FALSE(g.is_node(59));
    EXPECT_FALSE(g.is_node(-53));
}

TEST(insert_unique_nodes, directed_weighted_graph) {
    auto g = xtd::directed_weighted_graph<int, bool>();
    ASSERT_TRUE(g.empty());
    EXPECT_TRUE(g.insert_node(1));
    EXPECT_TRUE(g.is_node(1));
    EXPECT_TRUE(g.insert_node(2));
    EXPECT_TRUE(g.is_node(2));
    EXPECT_TRUE(g.insert_node(123));
    EXPECT_TRUE(g.is_node(123));
}

TEST(insert_duplicate_nodes, directed_weighted_graph) {
    auto g = xtd::directed_weighted_graph<int, bool>({1, 2, 123});
    ASSERT_TRUE(g.is_node(1));
    ASSERT_TRUE(g.is_node(2));
    ASSERT_TRUE(g.is_node(123));
    EXPECT_FALSE(g.insert_node(1));
    EXPECT_FALSE(g.insert_node(2));
    EXPECT_FALSE(g.insert_node(123));
}

TEST(begin_empty_graph, directed_weighted_graph) {
    auto g = xtd::directed_weighted_graph<int, bool>();
    EXPECT_EQ(g.end(), g.begin());
}

TEST(begin_graph_with_only_nodes, directed_weighted_graph) {
    auto g = xtd::directed_weighted_graph<int, bool>({1, 2, 3});
    EXPECT_NE(g.end(), g.begin());  // Note that dereferencing this iterator is invalid.
}

TEST(begin_graph_with_nodes_and_edges, directed_weighted_graph) {
    auto g = xtd::directed_weighted_graph<int, int>({1, 2, 3});
    ASSERT_TRUE(g.insert_edge(1, 2, 1));
    EXPECT_EQ(1, (*g.begin()).from);
    EXPECT_EQ(2, (*g.begin()).to);
    EXPECT_EQ(1, (*g.begin()).weight);
}

TEST(end_graph, directed_weighted_graph) {
    auto g = xtd::directed_weighted_graph<int, int>({1, 2, 3});
    ASSERT_TRUE(g.insert_edge(1, 2, 1));
    auto it = g.end();
    --it;
    EXPECT_EQ(1, (*g.begin()).from);
    EXPECT_EQ(2, (*g.begin()).to);
    EXPECT_EQ(1, (*g.begin()).weight);
}

TEST(is_connected, directed_weighted_graph) {
    auto g = xtd::directed_weighted_graph<std::string, int>({"A", "B", "C", "D"});
    ASSERT_TRUE(g.insert_edge("B", "D", 10));
    EXPECT_TRUE(g.is_connected("B", "D"));
    ASSERT_TRUE(g.insert_edge("B", "D", 0));
    EXPECT_TRUE(g.is_connected("B", "D"));
    ASSERT_TRUE(g.insert_edge("D", "B", 0));
    EXPECT_TRUE(g.is_connected("D", "B"));
    ASSERT_TRUE(g.insert_edge("A", "A", 0));
    EXPECT_TRUE(g.is_connected("A", "A"));
}

TEST(is_not_connected, directed_weighted_graph) {
    auto g = xtd::directed_weighted_graph<std::string, int>({"A", "B", "C", "D"});
    ASSERT_TRUE(g.insert_edge("B", "D", 10));
    EXPECT_FALSE(g.is_connected("D", "B"));
    EXPECT_FALSE(g.is_connected("D", "D"));
    EXPECT_FALSE(g.is_connected("B", "B"));
    EXPECT_FALSE(g.is_connected("A", "A"));
    EXPECT_FALSE(g.is_connected("C", "A"));
}

TEST(checking_connection_of_nonexisting_nodes, directed_weighted_graph) {
    auto g = xtd::directed_weighted_graph<std::string, int>({"A", "B", "C", "D"});
    EXPECT_THROW(static_cast<void>(g.is_connected("A", "AA")), std::runtime_error);
    EXPECT_THROW(static_cast<void>(g.is_connected("BB", "B")), std::runtime_error);
}

TEST(find_existing_edge, directed_weighted_graph) {
    auto g = xtd::directed_weighted_graph<int, bool>({1, 2, 123});
    ASSERT_TRUE(g.insert_edge(1, 2, "a"));
    EXPECT_NE(g.end(), g.find(1, 2, "a"));
}

TEST(insert_unique_edges, directed_weighted_graph) {
    auto g = xtd::directed_weighted_graph<int, std::string>({1, 2, 3, 4, 5});
    EXPECT_TRUE(g.insert_edge(1, 2, "a"));
    EXPECT_NE(g.end(), g.find(1, 2, "a"));
    EXPECT_TRUE(g.insert_edge(2, 3, "b"));
    EXPECT_NE(g.end(), g.find(2, 3, "b"));
    EXPECT_TRUE(g.insert_edge(2, 3, "c"));
    EXPECT_NE(g.end(), g.find(2, 3, "c"));
    EXPECT_TRUE(g.insert_edge(1, 1, "c"));
    EXPECT_NE(g.end(), g.find(1, 1, "c"));
}

TEST(insert_duplicate_edges, directed_weighted_graph) {
    auto g = xtd::directed_weighted_graph<int, std::string>({1, 2, 3, 4, 5});
    EXPECT_TRUE(g.insert_edge(1, 2, "a"));
    EXPECT_NE(g.end(), g.find(1, 2, "a"));
    EXPECT_FALSE(g.insert_edge(1, 2, "a"));
}

TEST(insert_edge_with_missing_nodes, directed_weighted_graph) {
    auto g = xtd::directed_weighted_graph<int, std::string>({1, 2, 3, 4, 5});
    EXPECT_THROW(g.insert_edge(1, 0, "a"), std::runtime_error);
    EXPECT_THROW(g.insert_edge(0, 5, "b"), std::runtime_error);
}

TEST(find_existing_edges, directed_weighted_graph) {
    auto g = xtd::directed_weighted_graph<int, int>({1, 2, 3, 4, 5});
    ASSERT_TRUE(g.insert_edge(1, 5, 1230));
    ASSERT_TRUE(g.insert_edge(1, 1, -100));
    ASSERT_TRUE(g.insert_edge(1, 5, 130));
    auto it1 = g.find(1, 5, 1230);
    EXPECT_NE(g.end(), it1);
    EXPECT_EQ(1, (*it1).from);
    EXPECT_EQ(5, (*it1).to);
    EXPECT_EQ(1230, (*it1).weight);
    auto it2 = g.find(1, 1, -100);
    EXPECT_NE(g.end(), it2);
    EXPECT_EQ(1, (*it2).from);
    EXPECT_EQ(1, (*it2).to);
    EXPECT_EQ(-100, (*it2).weight);
    auto it3 = g.find(1, 5, 130);
    EXPECT_NE(g.end(), it3);
    EXPECT_EQ(1, (*it3).from);
    EXPECT_EQ(5, (*it3).to);
    EXPECT_EQ(130, (*it3).weight);
}

TEST(find_nonexisting_edges, directed_weighted_graph) {
    auto g = xtd::directed_weighted_graph<int, int>({1, 2, 3, 4, 5});
    ASSERT_TRUE(g.insert_edge(1, 5, 1230));
    ASSERT_TRUE(g.insert_edge(1, 1, -100));
    ASSERT_TRUE(g.insert_edge(1, 5, 130));
    auto it1 = g.find(1, 5, 1231);
    EXPECT_EQ(g.end(), it1);
    auto it2 = g.find(1, 2, -100);
    EXPECT_EQ(g.end(), it2);
    auto it3 = g.find(5, 1, 130);
    EXPECT_EQ(g.end(), it3);
}

TEST(replace_node_with_nonexisting_node, directed_weighted_graph) {
    auto g = xtd::directed_weighted_graph<int, int>({1, 2});
    ASSERT_TRUE(g.insert_edge(1, 2, 0));
    EXPECT_TRUE(g.replace_node(2, 3));
    EXPECT_NE(g.end(), g.find(1, 3, 0));
}

TEST(replace_node_with_existing_node, directed_weighted_graph) {
    auto g = xtd::directed_weighted_graph<int, int>({1, 2, 3});
    ASSERT_TRUE(g.insert_edge(1, 2, 0));
    EXPECT_FALSE(g.replace_node(2, 3));
    EXPECT_EQ(g.end(), g.find(1, 3, 0));
}

TEST(replace_nonexisting_node, directed_weighted_graph) {
    auto g = xtd::directed_weighted_graph<int, int>({1, 2});
    ASSERT_TRUE(g.insert_edge(1, 2, 0));
    EXPECT_THROW(g.replace_node(0, 3), std::runtime_error);
}

TEST(removing_existing_node, directed_weighted_graph) {
    auto g = xtd::directed_weighted_graph<int, int>({1, 2});
    EXPECT_TRUE(g.erase_node(1));
    EXPECT_EQ(1, g.size());
    EXPECT_TRUE(g.erase_node(2));
    EXPECT_EQ(0, g.size());
}

TEST(removing_nonexisting_node, directed_weighted_graph) {
    auto g = xtd::directed_weighted_graph<int, int>();
    EXPECT_FALSE(g.erase_node(1));
    EXPECT_FALSE(g.erase_node(0));
}

TEST(removing_existing_node_with_edges, directed_weighted_graph) {
    auto g = xtd::directed_weighted_graph<int, int>({1, 2});
    ASSERT_TRUE(g.insert_edge(1, 2, 0));
    ASSERT_TRUE(g.insert_edge(2, 1, 0));
    EXPECT_TRUE(g.erase_node(1));
    EXPECT_EQ(g.end(), g.find(1, 2, 0));
    EXPECT_EQ(g.end(), g.find(2, 1, 0));
}

TEST(erase_existing_edge, directed_weighted_graph) {}
