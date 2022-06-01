/**
 * Checks that each adjacent element satisfies a given predicate.
 */

#pragma once

#include <algorithm>
#include <functional>

namespace xtd {

template <typename InputIt, typename BinaryPredicate>
auto adjacent_all_of(InputIt first, InputIt last, BinaryPredicate binary_pred) -> bool {
    return std::adjacent_find(first, last, std::not_fn(binary_pred)) == last;
}

}  // namespace xtd
