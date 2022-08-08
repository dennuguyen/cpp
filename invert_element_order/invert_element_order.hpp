#pragma once

#include <algorithm>
#include <iterator>
#include <map>
#include <vector>

namespace xtd {

template <typename Iter, typename T = typename std::iterator_traits<Iter>::value_type>
auto invert_element_order(Iter first, Iter last) -> void {
    auto const size = std::distance(first, last);

    auto elems_ascending_order = std::vector<T>(size);
    std::copy(first, last, elems_ascending_order.begin());
    std::sort(elems_ascending_order.begin(), elems_ascending_order.end());

    auto elems_descending_order = std::vector<T>(size);
    std::copy(first, last, elems_descending_order.begin());
    std::sort(elems_descending_order.rbegin(), elems_descending_order.rend());

    auto inverted_map = std::map<T, T>();
    for (std::size_t i = 0; i < size; i++) {
        inverted_map[elems_ascending_order[i]] = elems_descending_order[i];
    }

    while (first != last) {
        *first++ = inverted_map[*first];
    }
}

}  // namespace xtd