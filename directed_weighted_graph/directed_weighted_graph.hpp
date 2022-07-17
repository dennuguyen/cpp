#pragma once

#include <algorithm>
#include <cstddef>
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
    struct node_comparator;
    struct node_edge_comparator;
    using node_type = std::shared_ptr<N>;
    using neighbour_type = std::weak_ptr<N>;
    using edge_type = E;
    using pair_type = std::pair<neighbour_type, edge_type>;
    using set_type = std::set<pair_type, node_edge_comparator>;
    using map_type = std::map<node_type, set_type, node_comparator>;
    using size_type = typename map_type::size_type;

    struct value_type {
        N from;
        N to;
        E weight;
    };

    // Custom comparator for map_type.
    struct node_comparator {
        using is_transparent = void;
        constexpr auto operator()(node_type const& lhs, node_type const& rhs) const noexcept
            -> bool {
            return *lhs < *rhs;
        }
    };

    // Custom comparator for set_type.
    struct node_edge_comparator {
        using is_transparent = void;
        constexpr auto operator()(pair_type const& lhs, pair_type const& rhs) const noexcept
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
    //
    // Note: xtd::directed_weighted_graph<N, E>::iterator models std::bidirectional_iterator.
    class iterator {
       public:
        friend class directed_weighted_graph;
        using value_type = directed_weighted_graph<N, E>::value_type;
        using reference_type = value_type;
        using pointer = void;
        using difference_type = std::ptrdiff_t;
        using iterator_category = std::bidirectional_iterator_tag;
        using outer_iterator_type = typename map_type::const_iterator;
        using inner_iterator_type = typename set_type::const_iterator;

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
        auto operator++() noexcept -> iterator& {
            if (inner_ != outer_->second.end()) {
                if (++inner_ != outer_->second.end()) {
                    return *this;
                }
            }
            outer_++;
            inner_ = outer_ == last_ ? inner_iterator_type{} : outer_->second.begin();
            return *this;
        }

        auto operator++(int) noexcept -> iterator {
            auto temp = *this;
            ++*this;
            return temp;
        }

        // Advances *this to the previous element in the iterable list.
        auto operator--() noexcept -> iterator& {
            while (outer_ == last_ || inner_ == outer_->second.begin()) {
                --outer_;
                inner_ = outer_->second.end();
            }
            --inner_;
            return *this;

            if (inner_ != outer_->second.begin()) {
                if (--inner_ != outer_->second.begin()) {
                    return *this;
                }
            }
            outer_--;
            inner_ = outer_->second.end();
            return *this;
        }

        auto operator--(int) noexcept -> iterator {
            auto temp = *this;
            --*this;
            return temp;
        }

        // Returns true if *this and other are pointing to elements in the same iterable list, and
        // false otherwise.
        auto operator==(iterator const& other) const noexcept -> bool {
            return outer_ == other.outer_ && (outer_ == last_ || inner_ == other.inner_);
        }

        auto operator!=(iterator const& other) const noexcept -> bool { return !operator==(other); }

       private:
        // Constructs an iterator to a specific element in the directed_weighted_graph using the
        // first and last outer iterators of the directed_weighted_graph internal data structure.
        iterator(outer_iterator_type first, outer_iterator_type last) noexcept
            : iterator(last, first, first == last ? inner_iterator_type{} : first->second.begin()) {
        }

        // Constructs an iterator to a specific element in the directed_weighted_graph using the
        // outer and inner iterators of the directed_weighted_graph internal data structure.
        iterator(outer_iterator_type last, outer_iterator_type outer,
                 inner_iterator_type inner) noexcept
            : last_(last), outer_(outer), inner_(inner) {}

        outer_iterator_type last_;
        outer_iterator_type outer_;
        inner_iterator_type inner_;
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
        auto const& src_ptr = std::make_shared<N>(src);
        auto const& dst_ptr = neighbour_type(std::make_shared<N>(dst));

        // Get the iterator pointing to the pair with destination node and weight.
        auto it = internal_.at(src_ptr).find({dst_ptr, weight});
        return internal_.at(src_ptr).erase(it) != internal_.at(src_ptr).end();
    }

    // Erases the edge pointed to by i.
    //
    // Complexity is amortised constant time.
    //
    // Returns an iterator pointing to the element immediately after i prior to the element being
    // erased. If no such element exists, returns end().
    //
    // All iterators are invalidated.
    auto erase_edge(iterator i) noexcept -> iterator {
        return {internal_.erase(i.outer_), internal_.end()};
    }

    // Erases all edges between the iterators [i, s).
    //
    // Complexity is O(d), where d=std::distance(i, s).
    // Returns an iterator equivalent to s prior to the items iterated through being erased. If no
    // such element exists, returns end().
    //
    // All iterators are invalidated.
    auto erase_edge(iterator i, iterator s) noexcept -> iterator {
        return {internal_.erase(i.outer_, s.outer_), internal_.end()};
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
        return std::any_of(begin(), end(),
                           [&src, &dst](auto const& i) { return i.from == src && i.to == dst; });
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
        auto const& set = internal_[std::make_shared<N>(src)];
        auto vec = std::vector<E>(set.size());
        // Get the edge from set and put it into vec if the dst node matches.
        xtd::transform_if(
            set.begin(), set.end(), vec.begin(),
            [&dst](auto const& pair) { return *pair.first.lock() == dst; },
            [](auto const& pair) { return pair.second; });
        return vec;
    }

    // Returns an iterator pointing to an edge equivalent to value_type{src, dst, weight}, or
    // end() if no such edge exists.
    //
    // Complexity is O(log (n) + log (e)), where n is the number of stored nodes and e is the
    // number of stored edges.
    [[nodiscard]] auto find(N const& src, N const& dst, E const& weight) noexcept -> iterator {
        auto const& src_iter = find_node(src);
        if (src_iter == internal_.end()) {
            return end();
        }

        auto const& node_edge_set = internal_[src_iter->first];
        auto const& inner_iter = node_edge_set.find({std::make_shared<N>(dst), weight});
        if (inner_iter == node_edge_set.end()) {
            return end();
        }

        return {internal_.end(), src_iter, inner_iter};
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
        auto const& set = internal_.at(std::make_shared<N>(src));
        auto vec = std::vector<N>(set.size());
        // Get the dst node from set and copy to vec.
        std::transform(set.begin(), set.end(), vec.begin(),
                       [](auto const& pair) { return *pair.first.lock(); });
        return vec;
    }

    // Returns an iterator pointing to the first element in the container.
    [[nodiscard]] auto begin() const noexcept -> iterator {
        return {internal_.begin(), internal_.end()};
    }

    // Returns an iterator pointing to the first element in the container.
    auto begin() noexcept -> iterator { return {internal_.begin(), internal_.end()}; }

    // Returns an iterator denoting the end of the iterable list that begin() points to.
    //
    // [begin(), end()) shall denote a valid iterable list.
    [[nodiscard]] auto end() const noexcept -> iterator {
        return {internal_.end(), internal_.end()};
    }

    // Returns an iterator denoting the end of the iterable list that begin() points to.
    auto end() noexcept -> iterator { return {internal_.end(), internal_.end()}; }

    // Returns true if *this and other contain exactly the same nodes and edges, and false
    // otherwise.
    //
    // Complexity is O(n + e) where n is the sum of stored nodes in *this and other, and e is
    // the sum of stored edges in *this and other.
    [[nodiscard]] auto operator==(directed_weighted_graph const& other) const noexcept -> bool {
        // Graphs are the same if sorted nodes and sorted edges are the same.
        return nodes() == other.nodes() && edges() == other.edges();
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
        (void)g;
        return os;
    }

   private:
    // Helper function for copy-and-swap idiom.
    auto swap(directed_weighted_graph& g) noexcept -> directed_weighted_graph& {
        internal_.swap(g.internal_);
        return *this;
    }

    [[nodiscard]] auto find_node(std::shared_ptr<N> const& node) const noexcept
        -> map_type::const_iterator {
        return internal_.find(node);
    }

    auto find_node(std::shared_ptr<N> const& node) noexcept -> map_type::iterator {
        return internal_.find(node);
    }

    [[nodiscard]] auto find_node(N const& node) const noexcept -> map_type::const_iterator {
        return find_node(std::make_shared<N>(node));
    }

    auto find_node(N const& node) noexcept -> map_type::iterator {
        return find_node(std::make_shared<N>(node));
    }

    [[nodiscard]] auto find_node(std::weak_ptr<N> const& node) const noexcept
        -> map_type::const_iterator {
        return find_node(node.lock());
    }

    auto find_node(std::weak_ptr<N> const& node) noexcept -> map_type::iterator {
        return find_node(node.lock());
    }

    // Returns a sequence of all stored edges, sorted in ascending order.
    //
    // Complexity is O(n), where n is the number of stored nodes.
    [[nodiscard]] auto edges() const noexcept -> std::vector<E> {
        auto vec = std::vector<E>(internal_.size());
        for (auto const& [key, set] : internal_) {
            for (auto& [node, weight] : set) {
                auto const& weights_vec = weights(*key, *(node.lock()));
                std::merge(vec.begin(), vec.end(), weights_vec.begin(), weights_vec.end(),
                           vec.begin());
            }
        }
        return vec;
    }

    // Internal data structure uses a map to represent the directed_weighted_graph.
    map_type internal_;
};

}  // namespace xtd