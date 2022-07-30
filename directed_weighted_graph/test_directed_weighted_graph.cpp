#include "directed_weighted_graph.hpp"
#include "gtest/gtest.h"

TEST(directed_weighted_graph, empty_graph) {
    auto g = xtd::directed_weighted_graph<int, std::string>();
    EXPECT_TRUE(g.empty());
}

TEST(directed_weighted_graph, empty_initialiser_list) {
    auto g = xtd::directed_weighted_graph<int, std::string>({});
    EXPECT_TRUE(g.empty());
}

TEST(directed_weighted_graph, initialiser_list_with_ints) {
    auto g = xtd::directed_weighted_graph<int, std::string>({2, 3, 4});
    EXPECT_TRUE(g.is_node(2));
    EXPECT_TRUE(g.is_node(3));
    EXPECT_TRUE(g.is_node(4));
}

TEST(directed_weighted_graph, initialiser_list_with_strings) {
    auto g = xtd::directed_weighted_graph<std::string, std::string>({"abc", "cbd", "ebd"});
    EXPECT_TRUE(g.is_node("abc"));
    EXPECT_TRUE(g.is_node("cbd"));
    EXPECT_TRUE(g.is_node("ebd"));
}

TEST(directed_weighted_graph, initialiser_list_with_duplicates) {
    auto g =
        xtd::directed_weighted_graph<std::string, std::string>({"abc", "abc", "cbd", "ebd", "ebd"});
    EXPECT_EQ(3, g.size());
    EXPECT_TRUE(g.is_node("abc"));
    EXPECT_TRUE(g.is_node("cbd"));
    EXPECT_TRUE(g.is_node("ebd"));
}

TEST(directed_weighted_graph, empty_iterator_constructor) {
    auto il = std::initializer_list<int>();
    auto g = xtd::directed_weighted_graph<int, std::string>(il.begin(), il.end());
    EXPECT_TRUE(g.empty());
}

TEST(directed_weighted_graph, full_range_iterator_constructor) {
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

TEST(directed_weighted_graph, partial_range_iterator_constructor) {
    auto il = {"first", "second", "third", "fourth", "fifth", "sixth"};
    auto g = xtd::directed_weighted_graph<std::string, double>(il.begin() + 1, il.begin() + 4);
    EXPECT_FALSE(g.is_node("first"));
    EXPECT_TRUE(g.is_node("second"));
    EXPECT_TRUE(g.is_node("third"));
    EXPECT_TRUE(g.is_node("fourth"));
    EXPECT_FALSE(g.is_node("fifth"));
    EXPECT_FALSE(g.is_node("sixth"));
}

TEST(directed_weighted_graph, iterator_constructor_with_duplicates) {
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

TEST(directed_weighted_graph, copy_empty_graph) {
    auto g1 = xtd::directed_weighted_graph<int, int>();
    auto g2 = xtd::directed_weighted_graph<int, int>(g1);
    EXPECT_TRUE(g1.empty());
    EXPECT_TRUE(g2.empty());
}

TEST(directed_weighted_graph, copy_graph_with_only_nodes) {
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

TEST(directed_weighted_graph, copy_graph_with_nodes_and_edges) {
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

TEST(directed_weighted_graph, move_empty_graph) {
    auto g1 = xtd::directed_weighted_graph<int, int>();
    auto g2 = xtd::directed_weighted_graph<int, int>(std::move(g1));
    EXPECT_TRUE(g2.empty());
}

TEST(directed_weighted_graph, move_graph_with_only_nodes) {
    auto g1 = xtd::directed_weighted_graph<int, int>({3, 4, 6});
    auto g2 = xtd::directed_weighted_graph<int, int>(std::move(g1));
    EXPECT_TRUE(g2.is_node(3));
    EXPECT_TRUE(g2.is_node(4));
    EXPECT_TRUE(g2.is_node(6));
}

TEST(directed_weighted_graph, move_graph_with_nodes_and_edges) {
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

TEST(directed_weighted_graph, copy_assign_empty_graph) {
    auto g1 = xtd::directed_weighted_graph<int, int>();
    auto g2 = xtd::directed_weighted_graph<int, int>({0, -1, -2});
    g2 = g1;
    EXPECT_TRUE(g1.empty());
    EXPECT_TRUE(g2.empty());
}

TEST(directed_weighted_graph, copy_assign_graph_with_only_nodes) {
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

TEST(directed_weighted_graph, copy_assign_graph_with_nodes_and_edges) {
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

TEST(directed_weighted_graph, move_assign_empty_graph) {
    auto g1 = xtd::directed_weighted_graph<int, int>();
    auto g2 = xtd::directed_weighted_graph<int, int>({0, -1, -2});
    g2 = std::move(g1);
    EXPECT_TRUE(g2.empty());
}

TEST(directed_weighted_graph, move_assign_graph_with_only_nodes) {
    auto g1 = xtd::directed_weighted_graph<int, int>({0, -1, -2});
    auto g2 = xtd::directed_weighted_graph<int, int>();
    g2 = std::move(g1);
    EXPECT_TRUE(g2.is_node(0));
    EXPECT_TRUE(g2.is_node(-1));
    EXPECT_TRUE(g2.is_node(-2));
}

TEST(directed_weighted_graph, move_assign_graph_with_nodes_and_edges) {
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

TEST(directed_weighted_graph, clear_graph) {
    auto g = xtd::directed_weighted_graph<std::string, int>({"1", "2", "3"});
    ASSERT_FALSE(g.empty());
    g.clear();
    EXPECT_TRUE(g.empty());
}

TEST(directed_weighted_graph, empty_size) {
    auto g = xtd::directed_weighted_graph<std::string, int>();
    EXPECT_EQ(0, g.size());
}

TEST(directed_weighted_graph, nonempty_size) {
    auto g = xtd::directed_weighted_graph<std::string, int>({"1", "2"});
    EXPECT_EQ(2, g.size());
    g.insert_edge("1", "2", 1);  // Should have no effect on size.
    EXPECT_EQ(2, g.size());
}

TEST(directed_weighted_graph, is_empty) {
    auto g = xtd::directed_weighted_graph<std::string, int>();
    EXPECT_TRUE(g.empty());
}

TEST(directed_weighted_graph, is_not_empty) {
    auto g = xtd::directed_weighted_graph<std::string, int>({"1", "2"});
    EXPECT_FALSE(g.empty());
}

TEST(directed_weighted_graph, empty_nodes) {
    auto g = xtd::directed_weighted_graph<std::string, int>();
    auto v = std::vector<std::string>();
    EXPECT_EQ(g.nodes(), v);
}

TEST(directed_weighted_graph, sorted_nodes) {
    auto g = xtd::directed_weighted_graph<std::string, int>({"back", "apple", "bear", "arch", ""});
    auto v = std::vector<std::string>({"", "apple", "arch", "back", "bear"});
    EXPECT_EQ(g.nodes(), v);
}

TEST(directed_weighted_graph, is_node) {
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

TEST(directed_weighted_graph, is_not_node) {
    auto g = xtd::directed_weighted_graph<int, bool>({3, 4, 6, 8, -4, -1, 0, 23, 53, -59});
    EXPECT_FALSE(g.is_node(1));
    EXPECT_FALSE(g.is_node(2));
    EXPECT_FALSE(g.is_node(5));
    EXPECT_FALSE(g.is_node(7));
    EXPECT_FALSE(g.is_node(-2));
    EXPECT_FALSE(g.is_node(59));
    EXPECT_FALSE(g.is_node(-53));
}

TEST(directed_weighted_graph, insert_unique_nodes) {
    auto g = xtd::directed_weighted_graph<int, bool>();
    ASSERT_TRUE(g.empty());
    EXPECT_TRUE(g.insert_node(1));
    EXPECT_TRUE(g.is_node(1));
    EXPECT_TRUE(g.insert_node(2));
    EXPECT_TRUE(g.is_node(2));
    EXPECT_TRUE(g.insert_node(123));
    EXPECT_TRUE(g.is_node(123));
}

TEST(directed_weighted_graph, insert_duplicate_nodes) {
    auto g = xtd::directed_weighted_graph<int, bool>({1, 2, 123});
    ASSERT_TRUE(g.is_node(1));
    ASSERT_TRUE(g.is_node(2));
    ASSERT_TRUE(g.is_node(123));
    EXPECT_FALSE(g.insert_node(1));
    EXPECT_FALSE(g.insert_node(2));
    EXPECT_FALSE(g.insert_node(123));
}

TEST(directed_weighted_graph, begin_empty_graph) {
    auto g = xtd::directed_weighted_graph<int, bool>();
    EXPECT_EQ(g.end(), g.begin());
}

TEST(directed_weighted_graph, begin_graph_with_only_nodes) {
    auto g = xtd::directed_weighted_graph<int, bool>({1, 2, 3});
    EXPECT_EQ(g.end(), g.begin());  // Note that dereferencing this iterator is invalid.
}

TEST(directed_weighted_graph, begin_graph_with_nodes_and_edges) {
    auto g = xtd::directed_weighted_graph<int, int>({1, 2, 3});
    ASSERT_TRUE(g.insert_edge(1, 2, 1));
    EXPECT_EQ(1, (*g.begin()).from);
    EXPECT_EQ(2, (*g.begin()).to);
    EXPECT_EQ(1, (*g.begin()).weight);
}

TEST(directed_weighted_graph, end_graph) {
    auto g = xtd::directed_weighted_graph<int, int>({1, 2, 3});
    ASSERT_TRUE(g.insert_edge(1, 2, 1));
    auto it = g.end();
    --it;
    EXPECT_EQ(1, (*g.begin()).from);
    EXPECT_EQ(2, (*g.begin()).to);
    EXPECT_EQ(1, (*g.begin()).weight);
}

TEST(directed_weighted_graph, is_connected) {
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

TEST(directed_weighted_graph, is_not_connected) {
    auto g = xtd::directed_weighted_graph<std::string, int>({"A", "B", "C", "D"});
    ASSERT_TRUE(g.insert_edge("B", "D", 10));
    EXPECT_FALSE(g.is_connected("D", "B"));
    EXPECT_FALSE(g.is_connected("D", "D"));
    EXPECT_FALSE(g.is_connected("B", "B"));
    EXPECT_FALSE(g.is_connected("A", "A"));
    EXPECT_FALSE(g.is_connected("C", "A"));
}

TEST(directed_weighted_graph, checking_connection_of_nonexisting_nodes) {
    auto g = xtd::directed_weighted_graph<std::string, int>({"A", "B", "C", "D"});
    EXPECT_THROW(static_cast<void>(g.is_connected("A", "AA")), std::runtime_error);
    EXPECT_THROW(static_cast<void>(g.is_connected("BB", "B")), std::runtime_error);
}

TEST(directed_weighted_graph, find_existing_edge) {
    auto g = xtd::directed_weighted_graph<int, bool>({1, 2, 123});
    ASSERT_TRUE(g.insert_edge(1, 2, "a"));
    EXPECT_NE(g.end(), g.find(1, 2, "a"));
}

TEST(directed_weighted_graph, insert_unique_edges) {
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

TEST(directed_weighted_graph, insert_duplicate_edges) {
    auto g = xtd::directed_weighted_graph<int, std::string>({1, 2, 3, 4, 5});
    EXPECT_TRUE(g.insert_edge(1, 2, "a"));
    EXPECT_NE(g.end(), g.find(1, 2, "a"));
    EXPECT_FALSE(g.insert_edge(1, 2, "a"));
}

TEST(directed_weighted_graph, insert_edge_with_missing_nodes) {
    auto g = xtd::directed_weighted_graph<int, std::string>({1, 2, 3, 4, 5});
    EXPECT_THROW(g.insert_edge(1, 0, "a"), std::runtime_error);
    EXPECT_THROW(g.insert_edge(0, 5, "b"), std::runtime_error);
}

TEST(directed_weighted_graph, find_existing_edges) {
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

TEST(directed_weighted_graph, find_nonexisting_edges) {
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

TEST(directed_weighted_graph, replace_node_with_nonexisting_node) {
    auto g = xtd::directed_weighted_graph<int, int>({1, 2});
    ASSERT_TRUE(g.insert_edge(1, 2, 0));
    EXPECT_TRUE(g.replace_node(2, 3));
    EXPECT_EQ(g.end(), g.find(1, 2, 0));
    EXPECT_NE(g.end(), g.find(1, 3, 0));
}

TEST(directed_weighted_graph, replace_node_with_existing_node) {
    auto g = xtd::directed_weighted_graph<int, int>({1, 2, 3});
    ASSERT_TRUE(g.insert_edge(1, 2, 0));
    EXPECT_FALSE(g.replace_node(2, 3));
    EXPECT_EQ(g.end(), g.find(1, 2, 0));
    EXPECT_NE(g.end(), g.find(1, 3, 0));
}

TEST(directed_weighted_graph, replace_nonexisting_node) {
    auto g = xtd::directed_weighted_graph<int, int>({1, 2});
    ASSERT_TRUE(g.insert_edge(1, 2, 0));
    EXPECT_THROW(g.replace_node(0, 3), std::runtime_error);
}

TEST(directed_weighted_graph, merge_replacing_existing_node) {
    auto g = xtd::directed_weighted_graph<std::string, int>({"A", "B", "C", "D"});
    ASSERT_TRUE(g.insert_edge("A", "B", 1));
    ASSERT_TRUE(g.insert_edge("A", "C", 2));
    ASSERT_TRUE(g.insert_edge("A", "D", 3));
    ASSERT_TRUE(g.insert_edge("B", "B", 1));
    ASSERT_TRUE(g.insert_edge("C", "A", 1));
    g.merge_replace_node("A", "B");
    EXPECT_NE(g.end(), g.find("B", "B", 1));
    EXPECT_EQ(g.end(), g.find("A", "B", 1));
    EXPECT_NE(g.end(), g.find("B", "C", 2));
    EXPECT_EQ(g.end(), g.find("A", "C", 2));
    EXPECT_NE(g.end(), g.find("B", "D", 3));
    EXPECT_EQ(g.end(), g.find("A", "D", 3));
    EXPECT_NE(g.end(), g.find("C", "B", 1));
    EXPECT_EQ(g.end(), g.find("C", "A", 1));
}

TEST(directed_weighted_graph, merge_replacing_nonexisting_node) {
    auto g = xtd::directed_weighted_graph<std::string, int>({"A", "B"});
    EXPECT_THROW(g.merge_replace_node("A", "C"), std::runtime_error);
    EXPECT_THROW(g.merge_replace_node("C", "A"), std::runtime_error);
}

TEST(directed_weighted_graph, removing_existing_node) {
    auto g = xtd::directed_weighted_graph<int, int>({1, 2});
    EXPECT_TRUE(g.erase_node(1));
    EXPECT_EQ(1, g.size());
    EXPECT_TRUE(g.erase_node(2));
    EXPECT_EQ(0, g.size());
}

TEST(directed_weighted_graph, removing_nonexisting_node) {
    auto g = xtd::directed_weighted_graph<int, int>();
    EXPECT_FALSE(g.erase_node(1));
    EXPECT_FALSE(g.erase_node(0));
}

TEST(directed_weighted_graph, removing_existing_node_with_edges) {
    auto g = xtd::directed_weighted_graph<int, int>({1, 2});
    ASSERT_TRUE(g.insert_edge(1, 2, 0));
    ASSERT_TRUE(g.insert_edge(2, 1, 0));
    EXPECT_TRUE(g.erase_node(1));
    EXPECT_EQ(g.end(), g.find(1, 2, 0));
    EXPECT_EQ(g.end(), g.find(2, 1, 0));
}

TEST(directed_weighted_graph, erase_existing_edge) {
    auto g = xtd::directed_weighted_graph<std::string, int>({"A", "B"});
    ASSERT_TRUE(g.insert_edge("A", "B", 1));
    ASSERT_TRUE(g.insert_edge("B", "A", 1));
    ASSERT_TRUE(g.insert_edge("A", "B", 2));
    EXPECT_TRUE(g.erase_edge("A", "B", 1));
    EXPECT_EQ(g.end(), g.find("A", "B", 1));
    EXPECT_NE(g.end(), g.find("B", "A", 1));
    EXPECT_NE(g.end(), g.find("A", "B", 2));
    EXPECT_TRUE(g.erase_edge("B", "A", 1));
    EXPECT_EQ(g.end(), g.find("A", "B", 1));
    EXPECT_EQ(g.end(), g.find("B", "A", 1));
    EXPECT_NE(g.end(), g.find("A", "B", 2));
    EXPECT_TRUE(g.erase_edge("A", "B", 2));
    EXPECT_EQ(g.end(), g.find("A", "B", 1));
    EXPECT_EQ(g.end(), g.find("B", "A", 1));
    EXPECT_EQ(g.end(), g.find("A", "B", 2));
}

TEST(directed_weighted_graph, erase_nonexisting_edge) {
    auto g = xtd::directed_weighted_graph<std::string, int>({"A", "B"});
    EXPECT_FALSE(g.erase_edge("B", "A", 1));
    EXPECT_FALSE(g.erase_edge("A", "B", 2));
    ASSERT_TRUE(g.insert_edge("A", "A", 2));
    EXPECT_FALSE(g.erase_edge("A", "A", 1));
}

TEST(directed_weighted_graph, erase_nonexisting_edge_with_nonexisting_node) {
    auto g = xtd::directed_weighted_graph<std::string, int>({"A", "B"});
    EXPECT_THROW(g.erase_edge("C", "B", 1), std::runtime_error);
    EXPECT_THROW(g.erase_edge("A", "C", 1), std::runtime_error);
}

TEST(directed_weighted_graph, erase_existing_edge_with_single_iterator) {
    auto g = xtd::directed_weighted_graph<int, int>({1, 2, 3});
    ASSERT_TRUE(g.insert_edge(1, 1, 10));
    ASSERT_TRUE(g.insert_edge(1, 2, 10));
    ASSERT_TRUE(g.insert_edge(1, 3, 10));
    auto it1 = g.find(1, 1, 10);
    auto it2 = g.find(1, 2, 10);
    auto it3 = g.find(1, 3, 10);
    EXPECT_EQ(it2, g.erase_edge(it1));
    EXPECT_EQ(g.end(), g.find(1, 1, 10));
    EXPECT_EQ(it3, g.erase_edge(it2));
    EXPECT_EQ(g.end(), g.find(1, 2, 10));
    EXPECT_EQ(g.end(), g.erase_edge(it3));
    EXPECT_EQ(g.end(), g.find(1, 3, 10));
}

TEST(directed_weighted_graph, erase_nonexisting_edge_with_single_iterator) {
    auto g = xtd::directed_weighted_graph<int, int>({1, 2, 3});
    auto it1 = g.find(1, 1, 10);
    EXPECT_EQ(g.end(), g.erase_edge(it1));
    auto it2 = g.find(1, 2, 10);
    EXPECT_EQ(g.end(), g.erase_edge(it2));
    auto it3 = g.find(1, 3, 10);
    EXPECT_EQ(g.end(), g.erase_edge(it3));
}

TEST(directed_weighted_graph, iterator_is_lexicographically_ordered) {
    auto g = xtd::directed_weighted_graph<int, int>({1, 2, 3});
    ASSERT_TRUE(g.insert_edge(3, 2, 10));
    ASSERT_TRUE(g.insert_edge(3, 2, 4));
    ASSERT_TRUE(g.insert_edge(1, 3, 3));
    ASSERT_TRUE(g.insert_edge(1, 1, 10));
    ASSERT_TRUE(g.insert_edge(3, 3, 4));
    ASSERT_TRUE(g.insert_edge(2, 3, 3));
    ASSERT_TRUE(g.insert_edge(1, 2, 10));
    ASSERT_TRUE(g.insert_edge(1, 3, 10));
    auto iter = g.begin();
    EXPECT_EQ(iter++, g.find(1, 1, 10));
    EXPECT_EQ(iter++, g.find(1, 2, 10));
    EXPECT_EQ(iter++, g.find(1, 3, 3));
    EXPECT_EQ(iter++, g.find(1, 3, 10));
    EXPECT_EQ(iter++, g.find(2, 3, 3));
    EXPECT_EQ(iter++, g.find(3, 2, 4));
    EXPECT_EQ(iter++, g.find(3, 2, 10));
    EXPECT_EQ(iter++, g.find(3, 3, 4));
    EXPECT_EQ(iter, g.end());
}

TEST(directed_weighted_graph, erase_existing_edge_with_range_iterator) {
    auto g = xtd::directed_weighted_graph<int, int>({1, 2, 3});
    ASSERT_TRUE(g.insert_edge(3, 2, 10));
    ASSERT_TRUE(g.insert_edge(3, 2, 4));
    ASSERT_TRUE(g.insert_edge(1, 3, 3));
    ASSERT_TRUE(g.insert_edge(1, 1, 10));
    ASSERT_TRUE(g.insert_edge(3, 3, 4));
    ASSERT_TRUE(g.insert_edge(2, 3, 3));
    ASSERT_TRUE(g.insert_edge(1, 2, 10));
    ASSERT_TRUE(g.insert_edge(1, 3, 10));
    auto first = g.find(1, 3, 3);
    auto last = g.find(3, 2, 10);
    EXPECT_EQ(last, g.erase_edge(first, last));
    EXPECT_NE(g.end(), g.find(1, 1, 10));
    EXPECT_NE(g.end(), g.find(1, 2, 10));
    EXPECT_EQ(g.end(), g.find(1, 3, 3));
    EXPECT_EQ(g.end(), g.find(1, 3, 10));
    EXPECT_EQ(g.end(), g.find(2, 3, 3));
    EXPECT_EQ(g.end(), g.find(3, 2, 4));
    EXPECT_NE(g.end(), g.find(3, 2, 10));
    EXPECT_NE(g.end(), g.find(3, 3, 4));
}

TEST(directed_weighted_graph, erase_edge_with_range_iterator_with_nonexistent_begin) {
    auto g = xtd::directed_weighted_graph<int, int>({1, 2, 3});
    ASSERT_TRUE(g.insert_edge(3, 2, 10));
    ASSERT_TRUE(g.insert_edge(3, 2, 4));
    ASSERT_TRUE(g.insert_edge(1, 3, 3));
    ASSERT_TRUE(g.insert_edge(1, 1, 10));
    ASSERT_TRUE(g.insert_edge(3, 3, 4));
    ASSERT_TRUE(g.insert_edge(2, 3, 3));
    ASSERT_TRUE(g.insert_edge(1, 2, 10));
    ASSERT_TRUE(g.insert_edge(1, 3, 10));
    auto first = g.find(1, 3, 0);
    auto last = g.find(3, 3, 4);
    EXPECT_EQ(g.end(), g.erase_edge(first, last));
}

TEST(directed_weighted_graph, erase_edge_with_range_iterator_with_nonexistent_end) {
    auto g = xtd::directed_weighted_graph<int, int>({1, 2, 3});
    ASSERT_TRUE(g.insert_edge(3, 2, 10));
    ASSERT_TRUE(g.insert_edge(3, 2, 4));
    ASSERT_TRUE(g.insert_edge(1, 3, 3));
    ASSERT_TRUE(g.insert_edge(1, 1, 10));
    ASSERT_TRUE(g.insert_edge(3, 3, 4));
    ASSERT_TRUE(g.insert_edge(2, 3, 3));
    ASSERT_TRUE(g.insert_edge(1, 2, 10));
    ASSERT_TRUE(g.insert_edge(1, 3, 10));
    auto first = g.find(1, 3, 3);
    auto last = g.find(4, 5, 10);
    EXPECT_EQ(g.end(), g.erase_edge(first, last));
    EXPECT_NE(g.end(), g.find(1, 1, 10));
    EXPECT_NE(g.end(), g.find(1, 2, 10));
    EXPECT_EQ(g.end(), g.find(1, 3, 3));
    EXPECT_EQ(g.end(), g.find(1, 3, 10));
    EXPECT_EQ(g.end(), g.find(2, 3, 3));
    EXPECT_EQ(g.end(), g.find(3, 2, 4));
    EXPECT_EQ(g.end(), g.find(3, 2, 10));
    EXPECT_EQ(g.end(), g.find(3, 3, 4));
}

TEST(directed_weighted_graph, weights_of_graph_with_no_edges) {
    auto g = xtd::directed_weighted_graph<int, int>({1, 2, 3});
    auto v = g.weights(1, 2);
    EXPECT_EQ(0, v.size());
}

TEST(directed_weighted_graph, weights_of_graph_with_nonexisting_edges) {
    auto g = xtd::directed_weighted_graph<int, int>({1, 2, 3});
    ASSERT_TRUE(g.insert_edge(3, 2, 10));
    ASSERT_TRUE(g.insert_edge(3, 2, 4));
    ASSERT_TRUE(g.insert_edge(3, 2, 11));
    ASSERT_TRUE(g.insert_edge(1, 3, 3));
    auto v = g.weights(2, 3);
    EXPECT_EQ(0, v.size());
}

TEST(directed_weighted_graph, weights_of_graph_with_existing_edges) {
    auto g = xtd::directed_weighted_graph<int, int>({1, 2, 3});
    ASSERT_TRUE(g.insert_edge(3, 2, 10));
    ASSERT_TRUE(g.insert_edge(3, 2, 4));
    ASSERT_TRUE(g.insert_edge(3, 2, 11));
    ASSERT_TRUE(g.insert_edge(1, 3, 3));
    auto v = g.weights(3, 2);
    EXPECT_EQ(4, v.at(0));
    EXPECT_EQ(10, v.at(1));
    EXPECT_EQ(11, v.at(2));
    EXPECT_EQ(3, v.size());
}

TEST(directed_weighted_graph, weights_of_graph_with_nonexisting_nodes) {
    auto g = xtd::directed_weighted_graph<int, int>({1, 2, 3});
    EXPECT_THROW(g.weights(3, 4), std::runtime_error);
    EXPECT_THROW(g.weights(0, 1), std::runtime_error);
}

TEST(directed_weighted_graph, connections_of_graph_with_no_edges) {
    auto g = xtd::directed_weighted_graph<int, int>({1, 2, 3});
    auto v = g.connections(1);
    EXPECT_EQ(0, v.size());
}

TEST(directed_weighted_graph, connections_of_graph_with_existing_edges) {
    auto g = xtd::directed_weighted_graph<int, int>({1, 2, 3, 10});
    ASSERT_TRUE(g.insert_edge(3, 2, 10));
    ASSERT_TRUE(g.insert_edge(3, 10, 4));
    ASSERT_TRUE(g.insert_edge(3, 3, 11));
    ASSERT_TRUE(g.insert_edge(1, 3, 3));
    auto v = g.connections(3);
    EXPECT_EQ(2, v.at(0));
    EXPECT_EQ(3, v.at(1));
    EXPECT_EQ(10, v.at(2));
    EXPECT_EQ(3, v.size());
}

TEST(directed_weighted_graph, connections_of_graph_with_nonexisting_edges) {
    auto g = xtd::directed_weighted_graph<int, int>({1, 2, 3, 10});
    ASSERT_TRUE(g.insert_edge(3, 2, 10));
    ASSERT_TRUE(g.insert_edge(3, 10, 4));
    ASSERT_TRUE(g.insert_edge(3, 3, 11));
    ASSERT_TRUE(g.insert_edge(1, 3, 3));
    auto v = g.connections(2);
    EXPECT_EQ(0, v.size());
}

TEST(directed_weighted_graph, connections_of_graph_with_nonexisting_node) {
    auto g = xtd::directed_weighted_graph<int, int>({1, 2, 3, 10});
    EXPECT_THROW(g.connections(0), std::runtime_error);
}

TEST(directed_weighted_graph, comparing_empty_graphs) {
    auto g1 = xtd::directed_weighted_graph<int, int>();
    auto g2 = xtd::directed_weighted_graph<int, int>();
    EXPECT_TRUE(g1 == g2);
}

TEST(directed_weighted_graph, comparing_equal_graphs_with_only_nodes) {
    auto g1 = xtd::directed_weighted_graph<int, int>({1, 2, 3});
    auto g2 = xtd::directed_weighted_graph<int, int>({3, 2, 1});
    EXPECT_TRUE(g1 == g2);
}

TEST(directed_weighted_graph, comparing_equal_graphs_with_edges) {
    auto g1 = xtd::directed_weighted_graph<int, int>({1, 2, 3});
    auto g2 = xtd::directed_weighted_graph<int, int>({3, 2, 1});
    g1.insert_edge(1, 2, 1);
    g2.insert_edge(1, 2, 1);
    g1.insert_edge(1, 2, 3);
    g2.insert_edge(1, 2, 3);
    EXPECT_TRUE(g1 == g2);
}

TEST(directed_weighted_graph, comparing_unequal_graphs_with_both_graph_with_edges) {
    auto g1 = xtd::directed_weighted_graph<int, int>({1, 2, 3});
    auto g2 = xtd::directed_weighted_graph<int, int>({3, 2, 1});
    g1.insert_edge(1, 2, 1);
    g2.insert_edge(2, 1, 1);
    EXPECT_FALSE(g1 == g2);
}

TEST(directed_weighted_graph, comparing_unequal_graphs_with_one_graph_with_edges) {
    auto g1 = xtd::directed_weighted_graph<int, int>({1, 2, 3});
    auto g2 = xtd::directed_weighted_graph<int, int>({3, 2, 1});
    g2.insert_edge(1, 2, 1);
    g2.insert_edge(2, 1, 1);
    EXPECT_FALSE(g1 == g2);
}

TEST(directed_weighted_graph, outputting_graph) {
    auto g = xtd::directed_weighted_graph<int, int>({3, 2, 1});
    g.insert_edge(1, 2, 4);
    g.insert_edge(1, 2, 3);
    auto actual = std::stringstream();
    actual << g;
    auto output =
        "1 (\n"
        "  2 | 3\n"
        "  2 | 4\n"
        ")\n"
        "2 (\n"
        ")\n"
        "3 (\n"
        ")\n";
    EXPECT_EQ(output, actual.str());
}