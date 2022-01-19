#pragma once

namespace xtl {
// transform_if behaves like std::transform but has a unary_pred which conditionally executes unary_op.
template <typename InputIt, typename OutputIt, typename UnaryPredicate, typename UnaryOperation>
auto transform_if(InputIt first1, InputIt last1, OutputIt d_first, UnaryPredicate unary_pred, UnaryOperation unary_op) {
    while (first1 != last1) {
        if (unary_pred(*first1)) {
            *d_first++ = unary_op(*first1++);
        }
    }
    return d_first;
}
}  // namespace xtl