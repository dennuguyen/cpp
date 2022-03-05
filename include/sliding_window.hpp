/**
 * Generic sliding window algorithms.
 */

#pragma once

#include <algorithm>
#include <utility>

namespace xtl {

/**
 * Fast/slow sliding window algorithm implementation.
 *
 * Window length starts at min_window_length and grows.
 *
 * Window slides to right by 1.
 */
template <typename InputIt, typename OutputIt, typename BinaryPredicate, typename BinaryOperation>
auto sliding_window(InputIt first, InputIt last, OutputIt d_first, int min_window_length, BinaryPredicate binary_pred, BinaryOperation binary_op) -> OutputIt {
    int size = static_cast<int>(std::distance(first, last));
    while (min_window_length <= size) {
        auto start = first;
        auto end = first + min_window_length;
        while (true) {
            if (binary_pred(start, end)) {
                *d_first++ = binary_op(start, end);
            }

            start++;
            if (end++ == last) {
                break;
            }
        }
        min_window_length++;
    }
    return d_first;
}

}  // namespace xtl