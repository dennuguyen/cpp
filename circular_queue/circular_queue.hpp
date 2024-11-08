#include <ostream>

#pragma once

template <typename T, size_t capacity_>
class CircularQueue {
public:
    template <typename IteratorType>
    class iterator;
    using iterator_type = iterator<T*>;
    using const_iterator_type = iterator<T const*>;

    template <typename IteratorType>
    class iterator {
    public:
        friend class CircularQueue;

        auto operator*() -> T& { return *curr; }
        auto operator*() const -> T& { return *curr; }

        auto operator->() -> T* { return curr; }
        auto operator->() const -> T const* { return curr; }

        auto operator++() -> iterator& {
            advance(1);
            return *this;
        }

        auto operator++(int) -> iterator {
            auto temp = *this;
            ++*this;
            return temp;
        }

        auto operator--() -> iterator& {
            advance(-1);
            return *this;
        }

        auto operator--(int) -> iterator {
            auto temp = *this;
            --*this;
            return temp;
        }

        auto operator+(int offset) const -> iterator {
            iterator result = *this;
            result.advance(offset);
            return result;
        }

        auto operator-(int offset) const -> iterator {
            iterator result = *this;
            result.advance(-offset);
            return result;
        }

        auto operator<=>(iterator const& other) const -> std::strong_ordering {
            size_t diff1 = distance();
            size_t diff2 = other.distance();
            if (diff1 < diff2) return std::strong_ordering::less;
            if (diff1 > diff2) return std::strong_ordering::greater;
            return std::strong_ordering::equal;
        }

        friend auto operator<<(std::ostream& os, iterator const& it) -> std::ostream& {
            os << it.curr;
            return os;
        }

    private:
        iterator(IteratorType start, IteratorType head, IteratorType tail, size_t size)
            : iterator(start, head, tail, tail, size) {}
        iterator(IteratorType start, IteratorType head, IteratorType tail, IteratorType curr, size_t size)
            : start(start), head(head), tail(tail), curr(curr), size_(size) {}

        void advance(int offset) {
            auto temp = curr + offset;
            T* end = start + realCapacity;

            if (offset > 0) {
                // Increment wrap-around case.
                curr = (temp >= end) ? start + (temp - end) : temp;
            } else {
                // Decrement wrap-around case.
                curr = (temp < start) ? end - (start - temp) : temp;
            }
        }

        size_t distance() const {
            if (curr >= tail) {
                return curr - tail;
            } else {
                return realCapacity - (tail - start) + (curr - start);
            }
        }

        IteratorType start;
        IteratorType head;
        IteratorType tail;
        IteratorType curr;
        size_t size_;
    };

    CircularQueue() : CircularQueue({}) {}

    CircularQueue(std::initializer_list<T> il) : size_(0), head(data), tail(data) {
        for (auto const& e : il) push(e);
    }

    auto operator[](size_t index) -> T& { return *(wrap(tail + index)); }
    auto operator[](size_t index) const -> const T& { return *(wrap(tail + index)); }

    auto begin() -> iterator_type { return {data, head, tail, size_}; }
    auto end() -> iterator_type { return {data, head, tail, head, size_}; }

    auto begin() const -> const const_iterator_type { return {data, head, tail, size_}; }
    auto end() const -> const const_iterator_type { return {data, head, tail, head, size_}; }

    auto cbegin() const -> const const_iterator_type { return {data, head, tail, size_}; }
    auto cend() const -> const const_iterator_type { return {data, head, tail, head, size_}; }

    auto full() const -> bool { return size_ == capacity_; }
    auto empty() const -> bool { return size_ == 0; }
    auto size() const -> size_t { return size_; }
    auto capacity() const -> size_t { return capacity_; }

    auto front() -> T& { return *tail; }
    auto front() const -> T { return *tail; }

    auto push(T item) -> bool {
        if (full()) pop();
        *head = item;
        head = wrap(head + 1);
        if (size_ < capacity_) size_++;
        return true;
    }

    auto pop() -> bool {
        if (empty()) return false;
        tail = wrap(tail + 1);
        size_--;
        return true;
    }

    friend std::ostream& operator<<(std::ostream& os, CircularQueue const& cq) {
        for (std::size_t i = 0; i < cq.size_; ++i) {
            os << cq[i] << (i < cq.size_ - 1 ? ", " : "");
        }
        return os;
    }

private:
    auto wrap(T* ptr) -> T* {
        T* end = data + realCapacity;
        return ptr >= end ? data + (ptr - end) : ptr;
    }

    auto wrap(T* ptr) const -> const T* {
        const T* end = data + realCapacity;
        return ptr >= end ? data + (ptr - end) : ptr;
    }

    static constexpr size_t realCapacity = capacity_ + 1;  // +1 to store one-after-the-last-element.
    size_t size_;                                          // Required to handle size 0 and 1 cases.
    T data[realCapacity];                                  // Data is stored in reverse [tail, head].
    T* head;                                               // Newest element.
    T* tail;                                               // Oldest element.
};