#include "gtest/gtest.h"
#include "direct_weighted_graph.hpp"

TEST(copy_assign_empty_graph, direct_weighted_graph) {
    auto g1 = xtd::direct_weighted_graph<int, int>();
    auto g2 = xtd::direct_weighted_graph<int, int>({0, -1, -2});
    g2 = g1;
    EXPECT_TRUE(g1.empty());
    EXPECT_TRUE(g2.empty());
}

TEST(copy_assign_graph_with_only_nodes, direct_weighted_graph) {
    auto g1 = xtd::direct_weighted_graph<int, int>({3, 4, 6});
    auto g2 = xtd::direct_weighted_graph<int, int>({0, -1, -2});
    g2 = g1;
    EXPECT_TRUE(g1.is_node(3));
    EXPECT_TRUE(g1.is_node(4));
    EXPECT_TRUE(g1.is_node(6));
    EXPECT_TRUE(g2.is_node(3));
    EXPECT_TRUE(g2.is_node(4));
    EXPECT_TRUE(g2.is_node(6));
}

TEST(copy_assign_graph_with_nodes_and_edges, direct_weighted_graph) {
    auto g1 = xtd::direct_weighted_graph<int, int>({3, 4, 6});
    g1.insert_edge(3, 4, 1);
    g1.insert_edge(3, 6, 1);
    ASSERT_TRUE(g1.is_connected(3, 4));
    ASSERT_TRUE(g1.is_connected(3, 6));
    ASSERT_TRUE(g2.is_connected(3, 4));
    ASSERT_TRUE(g2.is_connected(3, 6));
    auto g2 = xtd::direct_weighted_graph<int, int>({0, -1, -2});
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

TEST(move_assign_empty_graph, direct_weighted_graph) {
    auto g1 = gdwg::graph<int, int>();
    auto g2 = gdwg::graph<int, int>({0, -1, -2});
    g2 = std::move(g1);
    EXPECT_TRUE(g2.empty());
}

TEST(move_assign_graph_with_only_nodes, direct_weighted_graph) {
    auto g1 = gdwg::graph<int, int>({0, -1, -2});
    auto g2 = gdwg::graph<int, int>();
    g2 = std::move(g1);
    EXPECT_TRUE(g2.is_node(0));
    EXPECT_TRUE(g2.is_node(-1));
    EXPECT_TRUE(g2.is_node(-2));
}

TEST(move_assign_graph_with_nodes_and_edges, direct_weighted_graph) {
    auto g1 = gdwg::graph<int, int>({3, 4, 6});
    g1.insert_edge(3, 4, 1);
    g1.insert_edge(3, 6, 1);
    ASSERT_TRUE(g1.is_connected(3, 4));
    ASSERT_TRUE(g1.is_connected(3, 6));
    auto g2 = gdwg::graph<int, int>({0, -1, -2});
    g2 = std::move(g1);
    EXPECT_TRUE(g2.is_node(3));
    EXPECT_TRUE(g2.is_node(4));
    EXPECT_TRUE(g2.is_node(6));
    EXPECT_TRUE(g2.is_connected(3, 4));
    EXPECT_TRUE(g2.is_connected(3, 6));
}

TEST(begin_with_empty_graph, direct_weighted_graph) {

}