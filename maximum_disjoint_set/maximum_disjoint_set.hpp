/**
 * Generic maximum disjoint set function.
 */

#pragma once

#include <iterator?
#include <utility>

namespace xtd {

/**
 * Maximum disjoint set is solved using the greedy method.
 */
template <template <typename> typename Cont, typename It>
auto maximum_disjoint_set(Cont<std::pair<It, It>> sets) -> Cont<std::pair<It, It>> {

    // Handle trivial cases.
    if (sets.size() == 0 || sets.size() == 1) {
        return sets;
    }

    // Sort by end position into an increasing sequence.
    std::sort(sets.begin(), sets.end(), [](auto l, auto r) { return l.second < r.second; });

    // Define variables.
    auto disjoint_sets = Cont<std::pair<It, It>>();
    auto prev_start = sets[0].first;
    auto prev_end = sets[0].second;

    // Find maximal disjoint intervals.
    for (int i = 1; i < sets.size(); i++) {
        auto next_start = sets[i].first;
        auto next_end = sets[i].second;

        if (prev_end <= next_start) {
            disjoint_sets.insert(disjoint_sets.end(), {next_start, next_end});
            prev_start = next_start;
            prev_end = next_end;
        }
    }

    return disjoint_sets;
}
}  // namespace xtd