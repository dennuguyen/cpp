#pragma once

namespace xtl {

template <class BidirectionalIterator>
void reverse_container(BidirectionalIterator first, BidirectionalIterator last, std::bidirectional_iterator_tag) {
    while (first != last && first != --last) {
        std::iter_swap(first++, last);
    }
}

template <class RandomAccessIterator>
void reverse_container(RandomAccessIterator first, RandomAccessIterator last, std::random_access_iterator_tag) {
    if (first == last) {
        return;
    }
    --last;
    while (first < last) {
        std::iter_swap(first++, last--);
    }
}

template <class Cont>
Cont reverse_container(Cont cont) {
    reverse_container(cont.begin(), cont.end(), std::__iterator_category(cont.begin()));
    return cont;
}

}  // namespace xtl