#pragma once

#include <algorithm>
#include <cstddef>

template <typename T, std::size_t capacity>
class CircularQueue {
public:
    template <typename IteratorType>
    class iterator;
    using iterator_type = iterator<T*>;
    using const_iterator_type = iterator<T const*>;

    // template <typename IteratorType>
    // class iterator {
    // public:
    //     friend class CircularQueue;
    //     using value_type = T;
    //     using pointer_type = T*;

    //     iterator(IteratorType head, IteratorType tail) : iterator(head, tail, tail) {}
    //     iterator(IteratorType head, IteratorType tail, IteratorType curr)
    //         : head(head), tail(tail), curr(curr), startedRHS(curr > tail) {}

    //     auto operator*() -> T& { return *curr; }
    //     auto operator*() const -> T& { return *curr; }

    //     auto operator->() -> T* { return curr; }
    //     auto operator->() const -> T const* { return curr; }

    //     auto operator++() -> iterator& {
    //         curr++;
    //         return *this;
    //     }

    //     auto operator++(int) -> iterator {
    //         auto temp = *this;
    //         ++*this;
    //         return temp;
    //     }

    //     auto operator--() -> iterator& {
    //         curr--;
    //         return *this;
    //     }

    //     auto operator--(int) -> iterator {
    //         auto temp = *this;
    //         --*this;
    //         return temp;
    //     }

    //     auto operator+(int offset) const -> iterator { return {head, tail, curr + offset}; }
    //     auto operator-(int offset) const -> iterator { return {head, tail, curr - offset}; }

    //     auto operator<=>(iterator const& other) const -> int {
    //         if (curr < other.curr || curr < other.curr) return -1;
    //         if (curr > other.curr || curr > other.curr) return 1;
    //         return 0;
    //     }

    //     auto operator==(iterator const& other) const -> bool = default;
    //     auto operator!=(iterator const& other) const -> bool = default;
    //     auto operator<(iterator const& other) const -> bool = default;
    //     auto operator>(iterator const& other) const -> bool = default;
    //     auto operator<=(iterator const& other) const -> bool = default;
    //     auto operator>=(iterator const& other) const -> bool = default;

    // private:
    //     IteratorType head;
    //     IteratorType tail;
    //     IteratorType curr;
    //     bool startedRHS;
    // };

    CircularQueue() : CircularQueue({}) {}

    CircularQueue(std::initializer_list<T> il) : size(0), head(data), tail(data) {
        for (auto const& e : il) push(e);
    }

    auto operator[](size_t index) -> T& { return *(wrap(tail + index)); }
    auto operator[](size_t index) const -> const T& { return *(wrap(tail + index)); }

    // auto begin() -> iterator_type { return {head, tail, tail}; }
    // auto end() -> iterator_type { return {head, tail, head}; }

    // auto begin() const -> const const_iterator_type { return {head, tail, tail}; }
    // auto end() const -> const const_iterator_type { return {head, tail, head}; }

    // auto cbegin() const -> const const_iterator_type { return {head, tail, tail}; }
    // auto cend() const -> const const_iterator_type { return {head, tail, head}; }

    auto full() const -> bool { return size == capacity; }
    auto empty() const -> bool { return size == 0; }

    auto front() -> T& { return *tail; }
    auto front() const -> T { return *tail; }

    auto push(T item) -> bool {
        if (full()) pop();
        *head = item;
        head = wrap(head + 1);
        if (size < capacity) size++;
        return true;
    }

    auto pop() -> bool {
        if (empty()) return false;
        tail = wrap(tail + 1);
        size--;
        return true;
    }

    friend std::ostream& operator<<(std::ostream& os, CircularQueue const& cq) {
        for (std::size_t i = 0; i < cq.size; ++i) {
            os << cq[i] << (i < cq.size - 1 ? ", " : "");
        }
        return os;
    }

private:
    auto wrap(T* ptr) -> T* {
        T* end = data + capacity;
        return ptr >= end ? data + (ptr - end) : ptr;
    }

    auto wrap(T* ptr) const -> const T* {
        const T* end = data + capacity;
        return ptr >= end ? data + (ptr - end) : ptr;
    }

    size_t size;       // Required to handle size 0 and 1 cases.
    T data[capacity];  // Data is stored in reverse [tail, head].
    T* head;           // One-after-the-element that was pushed.
    T* tail;           // Element to be popped first.
};