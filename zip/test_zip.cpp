#include <utility>
#include <vector>

#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include "zip.hpp"

namespace std {
template <typename U, typename V>
auto operator<<(std::ostream& os, std::pair<U, V> const& p) -> std::ostream& {
    os << "(" << p.first << ", " << p.second << ")";
    return os;
}
}  // namespace std

TEST_CASE("Zip(Cont1, Cont2)") {
    std::vector<int> vec1 = {0, 1, 2, 3, 4};
    std::vector<char> vec2 = {'a', 'b', 'c', 'd', 'e'};
    auto actual = Zip(vec1, vec2);
    CHECK(std::pair<int, char>(0, 'a') == actual[0]);
    CHECK(std::pair<int, char>(4, 'e') == actual[4]);
}

TEST_CASE("operator[]") {
    std::vector<int> vec1 = {0, 1, 2, 3, 4};
    std::vector<char> vec2 = {'a', 'b', 'c', 'd', 'e'};
    auto actual = Zip(vec1.begin(), vec1.end(), vec2.begin(), vec2.end());
    CHECK(std::pair<int, char>(0, 'a') == actual[0]);
    CHECK(std::pair<int, char>(1, 'b') == actual[1]);
    CHECK(std::pair<int, char>(2, 'c') == actual[2]);
    CHECK(std::pair<int, char>(3, 'd') == actual[3]);
    CHECK(std::pair<int, char>(4, 'e') == actual[4]);
}

TEST_CASE("begin()") {
    std::vector<int> vec1 = {0, 1, 2, 3, 4};
    std::vector<char> vec2 = {'a', 'b', 'c', 'd', 'e'};
    auto actual = Zip(vec1.begin(), vec1.end(), vec2.begin(), vec2.end());
    CHECK(std::pair<int, char>(0, 'a') == *actual.begin());
    CHECK(std::pair<int, char>(4, 'e') == *(actual.begin() + 4));
}

TEST_CASE("end()") {
    std::vector<int> vec1 = {0, 1, 2, 3, 4};
    std::vector<char> vec2 = {'a', 'b', 'c', 'd', 'e'};
    auto actual = Zip(vec1.begin(), vec1.end(), vec2.begin(), vec2.end());
    CHECK(std::pair<int, char>(0, 'a') == *(actual.end() - 5));
    CHECK(std::pair<int, char>(4, 'e') == *(actual.end() - 1));
}

TEST_CASE("begin() const") {
    std::vector<int> vec1 = {0, 1, 2, 3, 4};
    std::vector<char> vec2 = {'a', 'b', 'c', 'd', 'e'};
    auto const actual = Zip(vec1.begin(), vec1.end(), vec2.begin(), vec2.end());
    CHECK(std::pair<int, char>(0, 'a') == *actual.begin());
    CHECK(std::pair<int, char>(4, 'e') == *(actual.begin() + 4));
}

TEST_CASE("end() const") {
    std::vector<int> vec1 = {0, 1, 2, 3, 4};
    std::vector<char> vec2 = {'a', 'b', 'c', 'd', 'e'};
    auto const actual = Zip(vec1.begin(), vec1.end(), vec2.begin(), vec2.end());
    CHECK(std::pair<int, char>(0, 'a') == *(actual.end() - 5));
    CHECK(std::pair<int, char>(4, 'e') == *(actual.end() - 1));
}

TEST_CASE("cbegin()") {
    std::vector<int> vec1 = {0, 1, 2, 3, 4};
    std::vector<char> vec2 = {'a', 'b', 'c', 'd', 'e'};
    auto const actual = Zip(vec1.cbegin(), vec1.cend(), vec2.cbegin(), vec2.cend());
    CHECK(std::pair<int, char>(0, 'a') == *actual.cbegin());
    CHECK(std::pair<int, char>(4, 'e') == *(actual.cbegin() + 4));
}

TEST_CASE("cend()") {
    std::vector<int> vec1 = {0, 1, 2, 3, 4};
    std::vector<char> vec2 = {'a', 'b', 'c', 'd', 'e'};
    auto const actual = Zip(vec1.cbegin(), vec1.cend(), vec2.cbegin(), vec2.cend());
    CHECK(std::pair<int, char>(0, 'a') == *(actual.cend() - 5));
    CHECK(std::pair<int, char>(4, 'e') == *(actual.cend() - 1));
}

TEST_CASE("operator++()") {
    std::vector<int> vec1 = {0, 1, 2, 3, 4};
    std::vector<char> vec2 = {'a', 'b', 'c', 'd', 'e'};
    auto actual = Zip(vec1.begin(), vec1.end(), vec2.begin(), vec2.end());
    auto iter = actual.begin();
    CHECK(std::pair<int, char>(0, 'a') == *iter);
    iter++;
    CHECK(std::pair<int, char>(1, 'b') == *iter);
    iter++;
    CHECK(std::pair<int, char>(2, 'c') == *iter);
    iter++;
    CHECK(std::pair<int, char>(3, 'd') == *iter);
    iter++;
    CHECK(std::pair<int, char>(4, 'e') == *iter);
}

TEST_CASE("operator--()") {
    std::vector<int> vec1 = {0, 1, 2, 3, 4};
    std::vector<char> vec2 = {'a', 'b', 'c', 'd', 'e'};
    auto actual = Zip(vec1.begin(), vec1.end(), vec2.begin(), vec2.end());
    auto iter = actual.end();
    iter--;
    CHECK(std::pair<int, char>(4, 'e') == *iter);
    iter--;
    CHECK(std::pair<int, char>(3, 'd') == *iter);
    iter--;
    CHECK(std::pair<int, char>(2, 'c') == *iter);
    iter--;
    CHECK(std::pair<int, char>(1, 'b') == *iter);
    iter--;
    CHECK(std::pair<int, char>(0, 'a') == *iter);
}

TEST_CASE("operator->") {
    std::vector<int> vec1 = {0, 1, 2, 3, 4};
    std::vector<char> vec2 = {'a', 'b', 'c', 'd', 'e'};
    auto actual = Zip(vec1.begin(), vec1.end(), vec2.begin(), vec2.end());
    auto iter = actual.end() - 2;
    CHECK(3 == iter->first);
    CHECK('d' == iter->second);
}