#pragma once

#include <algorithm>
#include <cstddef>

template <typename T, std::size_t capacity>
class CircularQueue {
public:
    // template <typename IteratorType>
    // class iterator {
    // public:
    //     friend class CircularQueue;
    //     using value_type = T;
    //     using reference_type = T&;
    //     using pointer_type = T*;

    //     iterator(IteratorType head, IteratorType tail) : head(head), tail(tail) {}

    //     auto operator*() const -> value_type { return head; }
    //     auto operator->() const -> std::shared_ptr<value_type> { return std::make_shared<value_type>(operator*()); }

    //     auto operator++() -> iterator& {
    //         head++;
    //         return *this;
    //     }

    //     auto operator++(int) -> iterator {
    //         auto temp = *this;
    //         ++*this;
    //         return temp;
    //     }

    //     auto operator--() -> iterator& {
    //         head--;
    //         return *this;
    //     }

    //     auto operator--(int) -> iterator {
    //         auto temp = *this;
    //         --*this;
    //         return temp;
    //     }

    //     auto operator+(int offset) const -> iterator { return {head + offset}; }
    //     auto operator-(int offset) const -> iterator { return {head - offset}; }

    //     auto operator<=>(iterator const& other) const -> int {
    //         if (head < other.head || head < other.head) return -1;
    //         if (head > other.head || head > other.head) return 1;
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
    // };

    CircularQueue() : CircularQueue({}) {}

    CircularQueue(std::initializer_list<T> il) : size(std::min(il.size(), capacity)), head(size % capacity), tail(0) {
        std::copy(il.begin(), il.begin() + size, data);
    }

    auto operator[](size_t index) const -> T { return data[tailWrap(index)]; }

    auto begin() -> T* { return &data[tail]; }
    auto end() -> T* { return &data[head]; }

    auto begin() const -> const T* { return &data[tailWrap()]; }
    auto end() const -> const T* { return &data[headWrap()]; }

    auto cbegin() const -> const T* { return &data[head]; }
    auto cend() const -> const T* { return &data[tail]; }

    auto full() const -> bool { return size == capacity; }
    auto empty() const -> bool { return size == 0; }

    auto front() -> T& { return data[tail]; }
    auto front() const -> T { return data[tail]; }

    auto push(T item) -> bool {
        if (full()) pop();
        data[head] = item;
        head = headWrap(1);
        if (size < capacity) size++;
        return true;
    }

    auto pop() -> bool {
        if (empty()) return false;
        tail = tailWrap(1);
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
    auto tailWrap(size_t i = 0) const -> size_t { return (tail + i) % capacity; }
    auto headWrap(size_t i = 0) const -> size_t { return (head + i) % capacity; }

    T data[capacity];  // Data is stored in reverse [tail, head].
    size_t size;       // Required to handle size 0 and 1 cases.
    size_t head;       // Index of one-after-the-element that was pushed.
    size_t tail;       // Index of the element to be popped first.
};