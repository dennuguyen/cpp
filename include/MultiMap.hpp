/**
 * Multimap is a map with multiple keys that map to a single value. Pretty useful if a group of
 * objects has multiple type-identical properties.
 */

#ifndef MULTI_MAP_HPP_
#define MULTI_MAP_HPP_

#include <type_traits>
#include <unordered_map>

template <typename... Args>
class MultiMap {
   private:
    // Forward declaration for multiple arguments regardless of key and value.
    // This is useful in the variadic templated definition of MultiMapImple for recursion.
    template <typename... Kn>
    struct MultiMapImple;

    // Partial specialisation terminates MultiMapImple recursion.
    template <typename K, typename V>
    struct MultiMapImple<K, V> {
        using type = typename std::unordered_map<K, V>;
    };

    template <typename K, typename... Kn>
    struct MultiMapImple<K, Kn...> {
        // Inner MultiMapImple needs to have keywords typename and ::type to get base case's type or
        // type is overriden as just MultiMapImple<2nd last arg, last arg>.
        using type = typename std::unordered_map<K, typename MultiMapImple<Kn...>::type>;
    };

    typename MultiMapImple<Args...>::type map_;  // This map is where the magic happens.

   public:
    // Uses fold expression (, ...) to unpack Args then decltype inspects value category
    // of the unpacked "comma expression" which is basically the last arg. Good to note that value
    // is a prvalue so no temp object is created - O(1).
    using value = typename decltype((std::type_identity<Args>{}, ...))::type;

    // class iterator {
    //     std::unordered_map<last_key, value>::iterator current;
    // };

    MultiMap() = default;

    MultiMap(MultiMap const& other) : map_(other.map_) {}

    MultiMap(MultiMap&& other) noexcept {
        std::swap(map_, other.map_);
    }

    auto operator=(MultiMap const& other) noexcept -> MultiMap& {
        return MultiMap(other).swap(*this);
    }

    auto operator=(MultiMap&& other) noexcept -> MultiMap& {
        std::swap(map_, other.map_);
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
    auto operator[](T const& index) const -> decltype(map_[index])& {
        return map_[index];
    }

    template <typename T>
    auto operator[](T const& index) -> decltype(map_[index])& {
        return map_[index];
    }

    template <typename T>
    auto operator[](T const& index) const -> decltype(map_[index])& {
        return map_[index];
    }
};

#endif  // MULTI_MAP_HPP_