#ifndef MULTI_MAP_HPP_
#define MULTI_MAP_HPP_

/**
 * Multimap is a map with multiple keys that map to a single value. Pretty useful if a group of
 * objects has multiple type-identical properties.
 */

#include <type_traits>
#include <unordered_map>

// Because no compiler supports std::type_identity yet...
namespace std {
template <typename T>
struct type_identity {
    using type = T;
};
}  // namespace std

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

    typename MultiMapImple<Args...>::type map;

   public:
    // Uses fold expression (, ...) to unpack Args then decltype inspects value category
    // of the unpacked "comma expression" which is basically the last arg. Good to note that value
    // is a prvalue so no temp object is created - O(1).
    using value = typename decltype((std::type_identity<Args>{}, ...))::type;

    // class iterator {
    //     std::unordered_map<last_key, value>::iterator current;
    // };

    MultiMap() {}

    template <typename T>
    auto operator[](T const& index) -> decltype(map[index])& {
        return map[index];
    }

    template <typename T>
    auto operator[](T const& index) const -> decltype(map[index])& {
        return map[index];
    }
};

#endif  // MULTI_MAP_HPP_