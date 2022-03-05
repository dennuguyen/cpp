#pragma once

namespace xtl {

template <class BidirectionalIterator>
auto reverse_container(BidirectionalIterator first, BidirectionalIterator last, std::bidirectional_iterator_tag) -> void {
    while (first != last && first != --last) {
        std::iter_swap(first++, last);
    }
}

template <class RandomAccessIterator>
auto reverse_container(RandomAccessIterator first, RandomAccessIterator last, std::random_access_iterator_tag) -> void {
    if (first == last) {
        return;
    }
    --last;
    while (first < last) {
        std::iter_swap(first++, last--);
    }
}

template <class Cont>
auto reverse_container(Cont cont) -> Cont {
    reverse_container(cont.begin(), cont.end(), std::__iterator_category(cont.begin()));
    return cont;
}

}  // namespace xtl