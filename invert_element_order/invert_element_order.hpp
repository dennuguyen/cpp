#pragma once

#include <algorithm>
#include <iterator>
#include <map>
#include <vector>

namespace xtd {

template <typename Iter, typename T = typename std::iterator_traits<Iter>::value_type>
auto invert_element_order(Iter first, Iter last) -> void {
    auto sorted = std::vector<T>(std::distance(first, last));
    std::partial_sort_copy(first, last, sorted.rbegin(), sorted.rend());
    std::unique(sorted.begin(), sorted.end());

    auto inverted = std::map<T, T>();

    auto first1 = first;
    while (first1 != last) {
        inverted[*first1++];
    }

    auto first2 = inverted.begin();
    auto first3 = sorted.begin();
    while (first2 != inverted.end()) {
        (first2++)->second = *first3++;
    }

    while (first != last) {
        *first++ = inverted[*first];
    }
}

}  // namespace xtd