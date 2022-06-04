#pragma once

#include <algorithm>
#include <functional>

namespace xtd {

// Returns the index of the target value in a sorted container otherwise the index outside the range.
template<class ForwardIt, class T, class Compare = std::less<>>
auto binary_search_value(ForwardIt first, ForwardIt last, const T& value, Compare comp = Compare{}) -> T {
    return std::distance(first, std::lower_bound(first, last, value, comp));;
}

}  // namespace xtd
