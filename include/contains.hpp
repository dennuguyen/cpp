#pragma once

#include <algorithm>

namespace xtl {

// Returns true if the container 1 contains container 2.
template <class Container1, class Container2>
bool contains(Container1 cont1, Container2 cont2) {
    return std::search(cont1.begin(), cont1.end(), cont2.begin(), cont2.end()) != cont1.end();
}

// Returns true if the container 1 contains container 2 given a binary predicate.
template <class Container1, class Container2, class BinaryPredicate>
bool contains(Container1 cont1, Container2 cont2, BinaryPredicate binary_pred) {
    return std::search(cont1.begin(), cont1.end(), cont2.begin(), cont2.end(), binary_pred) != cont1.end();
}

} // namespace xtl