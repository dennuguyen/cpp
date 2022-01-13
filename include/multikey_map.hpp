/**
 * multikey_map is a map with multiple keys that map to a single value. Pretty useful if a group of
 * objects has multiple type-identical properties.
 */

#ifndef MULTI_MAP_HPP_
#define MULTI_MAP_HPP_

#include <type_traits>
#include <unordered_map>

template <typename... Args>
class multikey_map {
   private:
    // Forward declaration for multiple arguments regardless of key and value.
    // This is useful in the variadic templated definition of multikey_map_imple for recursion.
    template <typename... Kn>
    struct multikey_map_imple;

    // Partial specialisation terminates multikey_map_imple recursion.
    template <typename K, typename V>
    struct multikey_map_imple<K, V> {
        using type = typename std::unordered_map<K, V>;
    };

    template <typename K, typename... Kn>
    struct multikey_map_imple<K, Kn...> {
        // Inner multikey_map_imple needs to have keywords typename and ::type to get base case's type or
        // type is overriden as just multikey_map_imple<2nd last arg, last arg>.
        using type = typename std::unordered_map<K, typename multikey_map_imple<Kn...>::type>;
    };

    typename multikey_map_imple<Args...>::type map_;  // This map is where the magic happens.

   public:
    // Uses fold expression (, ...) to unpack Args then decltype inspects value category
    // of the unpacked "comma expression" which is basically the last arg. Good to note that value
    // is a prvalue so no temp object is created - O(1).
    using value = typename decltype((std::type_identity<Args>{}, ...))::type;

    // class iterator {
    //     std::unordered_map<last_key, value>::iterator current;
    // };

    multikey_map() = default;

    multikey_map(multikey_map const& other) : map_(other.map_) {}

    multikey_map(multikey_map&& other) noexcept {
        std::swap(map_, other.map_);
        other.clear();
    }

    auto operator=(multikey_map const& other) noexcept -> multikey_map& {
        return multikey_map(other).swap(*this);
    }

    auto operator=(multikey_map&& other) noexcept -> multikey_map& {
        std::swap(map_, other.map_);
        other.clear();
        return *this;
    }

    auto clear() noexcept -> void {
        map_.clear();
    }

    template <typename T>
    auto operator[](T const& index) -> decltype(map_[index])& {
        return map_[index];
    }

    template <typename T>
    auto operator[](T const& index) const -> decltype(map_[index]) {
        return map_[index];
    }

    template <typename T>
    auto at(T const& index) -> decltype(map_[index])& {
        return map_[index];
    }

    template <typename T>
    auto at(T const& index) const -> decltype(map_[index]) {
        return map_[index];
    }

   private:
    auto swap(multikey_map& other) noexcept -> multikey_map& {
        map_.swap(other.map_);
        return *this;
    }
};

#endif  // MULTI_MAP_HPP_