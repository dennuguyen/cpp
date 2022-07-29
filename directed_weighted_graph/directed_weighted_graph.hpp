#pragma once

#include <algorithm>
#include <cstddef>
#include <iostream>
#include <iterator>
#include <map>
#include <memory>
#include <set>
#include <utility>
#include <vector>

#include "transform_if.hpp"

namespace xtd {

template <typename N, typename E>
class directed_weighted_graph {
   public:
    template <typename OuterIteratorType, typename InnerIteratorType>
    class iterator;
    struct node_comparator;
    struct node_edge_comparator;
    using node_type = std::shared_ptr<N>;
    using edge_type = std::pair<std::weak_ptr<N>, E>;
    using map_value_type = std::set<edge_type, node_edge_comparator>;
    using map_key_type = std::map<node_type, map_value_type, node_comparator>;
    using size_type = typename map_key_type::size_type;
    using iterator_type =
        iterator<typename map_key_type::iterator, typename map_value_type::iterator>;
    using const_iterator_type =
        iterator<typename map_key_type::const_iterator, typename map_value_type::const_iterator>;

    struct value_type {
        N from;
        N to;
        E weight;

        auto operator<=>(value_type const& other) const noexcept -> int {
            if (from < other.from) return -1;
            if (from > other.from) return 1;
            if (to < other.to) return -1;
            if (to > other.to) return 1;
            if (weight < other.weight) return -1;
            if (weight > other.weight) return 1;
            return 0;
        }

        auto operator==(value_type const& other) const noexcept -> bool = default;
        auto operator!=(value_type const& other) const noexcept -> bool = default;

        friend auto operator<<(std::ostream& os, value_type const& v) noexcept -> std::ostream& {
            os << v.from << ": (" << v.to << ", " << v.weight << ")";
            return os;
        }
    };

    // Custom comparator for map_key_type.
    struct node_comparator {
        using is_transparent = void;
        constexpr auto operator()(node_type const& lhs, node_type const& rhs) const noexcept
            -> bool {
            return *lhs < *rhs;
        }
    };

    // Custom comparator for map_value_type.
    struct node_edge_comparator {
        using is_transparent = void;
        constexpr auto operator()(edge_type const& lhs, edge_type const& rhs) const noexcept
            -> bool {
            if (lhs.first.expired() || rhs.first.expired()) {
                return false;
            }

            // Compares nodes then compares edges if nodes are the same.
            return *lhs.first.lock() != *rhs.first.lock() ? *lhs.first.lock() < *rhs.first.lock()
                                                          : lhs.second < rhs.second;
        }
    };

    // Elements are lexicographically ordered by their source node, destination node, and edge
    // weight, in ascending order.
    //
    // Nodes without any connections are not traversed.
    template <typename OuterIteratorType, typename InnerIteratorType>
    class iterator {
       public:
        friend class directed_weighted_graph;
        using value_type = directed_weighted_graph<N, E>::value_type;
        using reference_type = value_type;
        using pointer = void;
        using difference_type = std::ptrdiff_t;
        using iterator_category = std::bidirectional_iterator_tag;

        // Pursuant to the requirements of std::forward_iterator, two value-initialised iterators
        // shall compare equal.
        iterator() = default;

        // Returns the current from, to, and weight.
        auto operator*() noexcept -> value_type {
            return {*outer_->first, *inner_->first.lock(), inner_->second};
        }

        // Returns the current from, to, and weight.
        auto operator*() const noexcept -> value_type {
            return {*outer_->first, *inner_->first.lock(), inner_->second};
        }

        // Advances *this to the next element in the iterable list.
        // Don't need to check if it is end iterator because it is invalid to call such methods.
        auto operator++() noexcept -> iterator& {
            inner_++;
            if (inner_ == outer_->second.end()) {
                skip_forward_empty_inner();
            }
            return *this;
        }

        auto operator++(int) noexcept -> iterator {
            auto temp = *this;
            ++*this;
            return temp;
        }

        // Advances *this to the previous element in the iterable list.
        auto operator--() noexcept -> iterator& {
            if (inner_ != outer_->second.begin()) {
                --inner_;
            } else {
                --outer_;
                skip_backward_empty_inner();
            }
            return *this;
        }

        auto operator--(int) noexcept -> iterator {
            auto temp = *this;
            --*this;
            return temp;
        }

        auto operator==(iterator const& other) const noexcept -> bool {
            return outer_ == other.outer_ && (outer_ == end_ || inner_ == other.inner_);
        }

        auto operator!=(iterator const& other) const noexcept -> bool { return !operator==(other); }

       private:
        auto skip_forward_empty_inner() -> InnerIteratorType {
            while (outer_ != end_ && inner_ == outer_->second.end()) {
                ++outer_;
                if (outer_ != end_) {
                    inner_ = outer_->second.begin();
                }
            }
            return inner_;
        }

        auto skip_backward_empty_inner() -> InnerIteratorType {
            while (outer_ != begin_ && outer_->second.begin() == outer_->second.end()) {
                --outer_;
            }
            if (outer_->second.begin() != outer_->second.end()) {
                inner_ = outer_->second.end();
                --inner_;
            }
            return inner_;
        }

        iterator(OuterIteratorType first, OuterIteratorType last) noexcept
            : iterator(first, last, first, first->second.begin()) {}

        iterator(OuterIteratorType first, OuterIteratorType last, OuterIteratorType outer,
                 InnerIteratorType inner) noexcept
            : begin_(first), end_(last), outer_(outer), inner_(inner) {
            skip_forward_empty_inner();
        }

        OuterIteratorType begin_;
        OuterIteratorType end_;
        OuterIteratorType outer_;
        InnerIteratorType inner_;
    };

    directed_weighted_graph() = default;

    directed_weighted_graph(std::initializer_list<N> il) noexcept
        : directed_weighted_graph(il.begin(), il.end()) {}

    template <typename InputIt>
    directed_weighted_graph(InputIt first, InputIt last) noexcept {
        std::for_each(first, last, [this](auto const& i) { insert_node(i); });
    }

    directed_weighted_graph(directed_weighted_graph&& other) noexcept {
        std::swap(internal_, other.internal_);
        other.clear();
    }

    auto operator=(directed_weighted_graph&& other) noexcept -> directed_weighted_graph& {
        std::swap(internal_, other.internal_);
        other.clear();
        return *this;
    }

    directed_weighted_graph(directed_weighted_graph const& other) noexcept
        : internal_(other.internal_) {}

    auto operator=(directed_weighted_graph const& other) noexcept -> directed_weighted_graph& {
        return directed_weighted_graph(other).swap(*this);
    }

    // Adds a new node with value value to the directed_weighted_graph if, and only if, there is no
    // node equivalent to value already stored.
    //
    // All iterators are invalidated.
    //
    // Returns true if the node is added to the directed_weighted_graph and false otherwise.
    auto insert_node(N const& value) noexcept -> bool {
        if (is_node(value) == false) {
            internal_[std::make_shared<N>(value)];
            return true;
        }
        return false;
    }

    // Adds a new edge representing src → dst with weight weight, if, and only if, there is no
    // edge equivalent to value_type{src, dst, weight} already stored. Note:⁠ Nodes are allowed
    // to be connected to themselves.
    //
    // All iterators are invalidated. Returns true if the node is added to the
    // directed_weighted_graph and false otherwise.
    //
    // Throws std::runtime_error if either of is_node(src) or is_node(dst) are false.
    auto insert_edge(N const& src, N const& dst, E const& weight) -> bool {
        if (is_node(src) == false || is_node(dst) == false) {
            throw std::runtime_error(
                "Cannot call xtd::directed_weighted_graph<N, E>::insert_edge when either src or "
                "dst node does not exist");
        }

        // Check if the edge doesn't already exist.
        if (find(src, dst, weight) == end()) {
            // Create required pointers to access the internal data structure.
            auto const& src_ptr = find_node(src)->first;
            auto const& dst_ptr = std::weak_ptr<N>(find_node(dst)->first);

            // Insert pointers into the set mapped to src_ptr.
            internal_[src_ptr].insert({dst_ptr, weight});
            return true;
        }
        return false;
    }

    // Replaces the original data, old_data, stored at this particular node by the replacement
    // data, new_data. Does nothing if new_data already exists as a node.
    //
    // All iterators are invalidated.
    //
    // Returns false if a node that contains value new_data already exists and true otherwise.
    //
    // Throws std::runtime_error if is_node(old_data) is false.
    auto replace_node(N const& old_data, N const& new_data) -> bool {
        if (is_node(old_data) == false) {
            throw std::runtime_error(
                "Cannot call xtd::directed_weighted_graph<N, E>::replace_node on a node that "
                "doesn't exist");
        }
        if (is_node(new_data) == false) {
            // By extracting the node handle, only the key gets repointed.
            auto node_handle = internal_.extract(std::make_shared<N>(old_data));
            node_handle.key() = std::make_shared<N>(new_data);
            internal_.insert(std::move(node_handle));
            return true;
        }
        return false;
    }

    // The node equivalent to old_data in the directed_weighted_graph are replaced with instances of
    // new_data. After completing, every incoming and outgoing edge of old_data becomes an
    // incoming/ougoing edge of new_data, except that duplicate edges shall be removed.
    //
    // All iterators are invalidated.
    //
    // Throws std::runtime_error if either of is_node(old_data) or is_node(new_data) are false.
    auto merge_replace_node(N const& old_data, N const& new_data) -> void {
        if (is_node(old_data) == false || is_node(new_data) == false) {
            throw std::runtime_error(
                "Cannot call xtd::directed_weighted_graph<N, E>::merge_replace_node on old or new "
                "data if they don't exist in the directed_weighted_graph");
        }
        // By extracting the node handle, only the key gets repointed.
        auto node_handle = internal_.extract(std::make_shared<N>(old_data));
        node_handle.key() = std::make_shared<N>(new_data);
        internal_.insert(std::move(node_handle));
    }

    // Erases all nodes equivalent to value, including all incoming and outgoing edges.
    //
    // Returns true if value was removed; false otherwise.
    //
    // All iterators are invalidated.
    auto erase_node(N const& value) noexcept -> bool {
        if (is_node(value) == false) {
            return false;
        }
        // Check all nodes and remove the value if there exists an edge.
        for (auto& [key, set] : internal_) {
            auto it = std::find_if(set.begin(), set.end(), [&value](auto const& pair) {
                return *pair.first.lock() == value;
            });
            if (it != set.end()) {
                set.erase(it);
            }
        }
        // Remove key last.
        internal_.erase(find_node(value));
        return true;
    }

    // Erases an edge representing src → dst with weight weight.
    //
    // Returns true if an edge was removed; false otherwise.
    //
    // All iterators are invalidated.
    //
    // Throws std::runtime_error if either is_node(src) or is_node(dst) is false.
    //
    // Complexity is O(log (n) + e), where n is the total number of stored nodes and e is the
    // total number of stored edges.
    auto erase_edge(N const& src, N const& dst, E const& weight) -> bool {
        if (is_node(src) == false || is_node(dst) == false) {
            throw std::runtime_error(
                "Cannot call xtd::directed_weighted_graph<N, E>::erase_edge on src or dst if they "
                "don't exist in the directed_weighted_graph");
        }

        auto iter = find(src, dst, weight);  // O(log(n) + O(log(e))).
        if (iter == end()) {
            return false;
        }

        iter.outer_->second.erase(iter.inner_);  // O(1).
        return true;
    }

    // Erases the edge pointed to by i.
    //
    // Complexity is amortised constant time.
    //
    // Returns an iterator pointing to the element immediately after i prior to the element being
    // erased. If no such element exists, returns end().
    //
    // All iterators are invalidated.
    auto erase_edge(iterator_type i) noexcept -> iterator_type {
        return i.outer_ == i.end_
                   ? end()
                   : iterator(i.begin_, i.end_, i.outer_, i.outer_->second.erase(i.inner_));
    }

    // Erases all edges between the iterators [i, s).
    //
    // Complexity is O(d), where d=std::distance(i, s).
    // Returns an iterator equivalent to s prior to the items iterated through being erased. If no
    // such element exists, returns end().
    //
    // All iterators are invalidated.
    auto erase_edge(iterator_type i, iterator_type s) noexcept -> iterator_type {
        while (i != s && i != end()) {
            i = erase_edge(i);
        }
        return i;
    }

    [[nodiscard]] auto size() const noexcept -> size_type { return internal_.size(); }

    auto clear() noexcept -> void { internal_.clear(); }

    // Returns true if a node equivalent to value exists in the directed_weighted_graph, and false
    // otherwise.
    //
    // Complexity is O(log (n)) time.
    [[nodiscard]] auto is_node(N const& value) const -> bool {
        // Make value a pointer so it can be used in node_comparator.
        return internal_.contains(std::make_shared<N>(value));
    }

    // Returns true if there are no nodes in the directed_weighted_graph, and false otherwise.
    [[nodiscard]] auto empty() const noexcept -> bool { return internal_.empty(); }

    // Returns true if an edge src → dst exists in the directed_weighted_graph, and false otherwise.
    //
    // Throws std::runtime_error if either of is_node(src) or is_node(dst) are false.
    [[nodiscard]] auto is_connected(N const& src, N const& dst) const -> bool {
        if (is_node(src) == false || is_node(dst) == false) {
            throw std::runtime_error(
                "Cannot call xtd::directed_weighted_graph<N, E>::is_connected if src or dst node "
                "don't exist in the directed_weighted_graph");
        }
        auto const& src_iter = find_node(src);
        return std::any_of(src_iter->second.begin(), src_iter->second.end(),
                           [&dst](auto const& i) { return *(i.first.lock()) == dst; });
    }

    // Returns a sequence of all stored nodes, sorted in ascending order.
    //
    // Complexity is O(n), where n is the number of stored nodes.
    [[nodiscard]] auto nodes() const noexcept -> std::vector<N> {
        auto vec = std::vector<N>(internal_.size());
        // Get src nodes from internal_ and put it into vec.
        std::transform(internal_.begin(), internal_.end(), vec.begin(),
                       [](auto const& pair) { return *pair.first; });
        return vec;
    }

    // Returns a sequence of weights from src to dst, sorted in ascending order.
    //
    // Complexity is O(log (n) + e), where n is the number of stored nodes and e is the number of
    // stored edges.
    //
    // Throws std::runtime_error if either of is_node(src) or is_node(dst) are false.
    [[nodiscard]] auto weights(N const& src, N const& dst) const -> std::vector<E> {
        if (is_node(src) == false || is_node(dst) == false) {
            throw std::runtime_error(
                "Cannot call xtd::directed_weighted_graph<N, E>::weights if src or dst node don't "
                "exist in the directed_weighted_graph");
        }

        auto const& edges = find_node(src)->second;  // O(log(n)).
        auto vec = std::vector<E>(edges.size());
        xtd::transform_if(
            edges.begin(), edges.end(), vec.begin(),
            [&dst](auto const& pair) { return *pair.first.lock() == dst; },
            [](auto const& pair) { return pair.second; });  // O(e).
        vec.shrink_to_fit();
        return vec;
    }

    // Returns an iterator pointing to an edge equivalent to value_type{src, dst, weight}, or
    // end() if no such edge exists.
    //
    // Complexity is O(log (n) + log (e)), where n is the number of stored nodes and e is the
    // number of stored edges.
    [[nodiscard]] auto find(N const& src, N const& dst, E const& weight) noexcept -> iterator_type {
        auto const& src_iter = find_node(src);
        if (src_iter == internal_.end()) {
            return end();
        }

        auto const& edges = internal_[src_iter->first];
        auto const& edge_iter = edges.find({std::make_shared<N>(dst), weight});
        if (edge_iter == edges.end()) {
            return end();
        }

        return {internal_.begin(), internal_.end(), src_iter, edge_iter};
    }

    // Returns a sequence of nodes (found from any immediate outgoing edge) connected to src,
    // sorted in ascending order, with respect to the connected nodes.
    //
    // Complexity is O(log (n) + e), where e is the number of outgoing edges associated with
    // src.
    //
    // Throws std::runtime_error if is_node(src) is false.
    [[nodiscard]] auto connections(N const& src) const -> std::vector<N> {
        if (is_node(src) == false) {
            throw std::runtime_error(
                "Cannot call xtd::directed_weighted_graph<N, E>::connections if src doesn't "
                "exist "
                "in the directed_weighted_graph");
        }
        auto const& edges = find_node(src)->second;  // O(log(n)).
        auto vec = std::vector<N>(edges.size());
        std::transform(edges.begin(), edges.end(), vec.begin(),
                       [](auto const& pair) { return *pair.first.lock(); });  // O(e).
        vec.shrink_to_fit();
        return vec;
    }

    [[nodiscard]] auto begin() const noexcept -> const_iterator_type {
        return {internal_.begin(), internal_.end()};
    }

    auto begin() noexcept -> iterator_type { return {internal_.begin(), internal_.end()}; }

    [[nodiscard]] auto end() const noexcept -> const_iterator_type {
        return {internal_.end(), internal_.end()};
    }

    auto end() noexcept -> iterator_type { return {internal_.end(), internal_.end()}; }

    auto cbegin() const noexcept -> const_iterator_type {
        return {internal_.cbegin(), internal_.cend()};
    }

    auto cend() const noexcept -> const_iterator_type {
        return {internal_.cend(), internal_.cend()};
    }

    // Returns true if *this and other contain exactly the same nodes and edges, and false
    // otherwise.
    //
    // Complexity is O(n + e) where n is the sum of stored nodes in *this and other, and e is
    // the sum of stored edges in *this and other.
    [[nodiscard]] auto operator==(directed_weighted_graph const& other) const noexcept -> bool {
        if (nodes() != other.nodes()) {
            return false;
        }
        if ((cbegin() == cend()) ^ (other.cbegin() == other.cend())) {
            return false;
        }
        return std::equal(cbegin(), cend(), other.cbegin());
    }

    // Behaves as a formatted output function of os.
    //
    // Returns os.
    //
    // The format is specified thusly:
    // 		[source_node1] [edges1]
    // 		[source_node2] [edges2]
    // 		...
    // 		[source_noden] [edgesn]
    // [source_node1], …, [source_noden] are placeholders for all nodes that the
    // directed_weighted_graph stores, sorted in ascending order. [edges1], …, [edgesn] are
    // placeholders for:
    // 		(
    // 		 [noden_connected_node1] | [weight]
    // 		 [noden_connected_node2] | [weight]
    // 		 ...
    // 		 [noden_connected_noden] | [weight]
    // 		)
    // where [noden_conencted_node1] | [weight], …, [noden_connected_noden] | [weight] are
    // placeholders for each node’s connections and corresponding weight, also sorted in
    // ascending order. Note: If a node doesn’t have any connections, then its corresponding
    // [edgesn] should be a line-separated pair of parentheses.
    friend auto operator<<(std::ostream& os, directed_weighted_graph const& g) noexcept
        -> std::ostream& {
        for (auto const& i : g) {
            // std::cout << i << std::endl;
        }
        // std::for_each(cbegin(), cend(), [](auto const& i){
        //     std::cout << i << std::endl;
        // });
        // std::for_each(other.cbegin(), other.cend(), [](auto const& i){
        //     std::cout << i << std::endl;
        // });
        return os;
    }

   private:
    // Helper function for copy-and-swap idiom.
    auto swap(directed_weighted_graph& g) noexcept -> directed_weighted_graph& {
        internal_.swap(g.internal_);
        return *this;
    }

    [[nodiscard]] auto find_node(std::shared_ptr<N> const& node) const noexcept
        -> map_key_type::const_iterator {
        return internal_.find(node);
    }

    auto find_node(std::shared_ptr<N> const& node) noexcept -> map_key_type::iterator {
        return internal_.find(node);
    }

    [[nodiscard]] auto find_node(N const& node) const noexcept -> map_key_type::const_iterator {
        return find_node(std::make_shared<N>(node));
    }

    auto find_node(N const& node) noexcept -> map_key_type::iterator {
        return find_node(std::make_shared<N>(node));
    }

    [[nodiscard]] auto find_node(std::weak_ptr<N> const& node) const noexcept
        -> map_key_type::const_iterator {
        return find_node(node.lock());
    }

    auto find_node(std::weak_ptr<N> const& node) noexcept -> map_key_type::iterator {
        return find_node(node.lock());
    }

    // Internal data structure uses a map to represent the directed_weighted_graph.
    map_key_type internal_;
};

}  // namespace xtd