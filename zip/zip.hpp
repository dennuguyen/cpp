#pragma once

#include <functional>
#include <ranges>
#include <type_traits>

template <typename It1, typename It2>
class Zip {
public:
    template <typename FirstIteratorType, typename SecondIteratorType>
    class iterator;
    using value_type = std::pair<typename It1::value_type, typename It2::value_type>;
    using iterator_type = iterator<It1, It2>;
    using const_iterator_type = iterator<typename std::add_const<It1>::type, typename std::add_const<It2>::type>;

    template <typename FirstIteratorType, typename SecondIteratorType>
    class iterator {
    public:
        friend class Zip;
        using value_type = Zip::value_type;

        iterator(FirstIteratorType iter1, SecondIteratorType iter2) : iter1(iter1), iter2(iter2) {}

        auto operator*() const -> value_type { return {*iter1, *iter2}; }
        auto operator->() const -> std::shared_ptr<value_type> { return std::make_shared<value_type>(operator*()); }

        auto operator++() -> iterator& {
            iter1++;
            iter2++;
            return *this;
        }

        auto operator++(int) -> iterator {
            auto temp = *this;
            ++*this;
            return temp;
        }

        auto operator--() -> iterator& {
            iter1--;
            iter2--;
            return *this;
        }

        auto operator--(int) -> iterator {
            auto temp = *this;
            --*this;
            return temp;
        }

        auto operator+(int offset) const -> iterator { return iterator(iter1 + offset, iter2 + offset); }
        auto operator-(int offset) const -> iterator { return iterator(iter1 - offset, iter2 - offset); }

        auto operator<=>(iterator const& other) const -> int {
            if (iter1 < other.iter1 || iter2 < other.iter2) return -1;
            if (iter1 > other.iter1 || iter2 > other.iter2) return 1;
            return 0;
        }

        auto operator==(iterator const& other) const -> bool = default;
        auto operator!=(iterator const& other) const -> bool = default;

    private:
        FirstIteratorType iter1;
        SecondIteratorType iter2;
    };

    Zip(It1 first1, It1 last1, It2 first2, It2 last2) : first1(first1), last1(last1), first2(first2), last2(last2) {}

    auto operator[](size_t index) const -> value_type { return {first1[index], first2[index]}; }

    auto begin() -> iterator_type { return {first1, first2}; }
    auto end() -> iterator_type { return {last1, last2}; }

    auto begin() const -> const_iterator_type { return {first1, first2}; }
    auto end() const -> const_iterator_type { return {last1, last2}; }

    auto cbegin() const -> const_iterator_type { return {first1, first2}; }
    auto cend() const -> const_iterator_type { return {last1, last2}; }

private:
    // First container.
    It1 first1;
    It1 last1;

    // Second container.
    It2 first2;
    It2 last2;
};