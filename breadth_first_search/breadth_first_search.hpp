#pragma once

#include <queue>
#include <vector>

#include <iostream>
namespace xtd {

template<template <typename, typename> typename Graph, typename N, typename E>
auto breadth_first_search(Graph<N, E> const& graph, N const& from, N const& to) -> std::vector<std::vector<N>> {
    auto paths = std::vector<std::vector<N>>();

    auto current_path = std::vector<N>();
    current_path.emplace_back(from);

    auto search_queue = std::queue<std::pair<N, std::vector<N>>>();
    search_queue.push(std::make_pair(from, current_path));

    while (!search_queue.empty()) {
        auto const [current_node, current_path] = search_queue.front();
        search_queue.pop();

        // Found the destination.
        if (current_node == to) {
            paths.push_back(current_path);
        }

        for (auto const& adjacent_node : graph.connections(current_node)) {
            auto new_path = std::vector<N>(current_path);
            new_path.emplace_back(adjacent_node);
            search_queue.push(std::make_pair(adjacent_node, new_path));
        }
    }

    return paths;
}

}  // namespace xtd