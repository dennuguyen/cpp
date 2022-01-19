/**
 * multikey_map is a map with multiple keys that map to a single value. Pretty useful if a group of
 * objects has multiple type-identical properties.
 */

#pragma once

#include <type_traits>
#include <unordered_map>

namespace xtl {

// Because no compiler supports std::type_identity yet...
namespace std {
template <typename T>
struct type_identity {
    using type = T;
};
}  // namespace std

template <typename... Args>
class multikey_map {
   private:
    // Forward declaration for multiple arguments regardless of key and value.
    // This is useful in the variadic templated definition of multikey_map_imple for recursion.
    template <typename... Tn>
    struct multikey_map_imple;

    // Partial specialisation terminates multikey_map_imple recursion.
    template <typename Key, typename T>
    struct multikey_map_imple<Key, T> {
        using type = typename std::unordered_map<Key, T>;
    };

    template <typename Key, typename... Tn>
    struct multikey_map_imple<Key, Tn...> {
        // Inner multikey_map_imple needs to have keywords typename and ::type to get base case's type or
        // type is overriden as just multikey_map_imple<2nd last arg, last arg>.
        using type = typename std::unordered_map<Key, typename multikey_map_imple<Tn...>::type>;
    };

    typename multikey_map_imple<Args...>::type map_;  // This map is where the magic happens.

   public:
    using key_type = std::tuple<const Args...>;
    using value_type = std::tuple<const Args...>;
    using reference = value_type&;
    using const_reference = const value_type&;

    // Uses fold expression (, ...) to unpack Args then decltype inspects value category
    // of the unpacked "comma expression" which is basically the last arg. Good to note that mapped_type
    // is a prvalue so no temp object is created - O(1).
    using mapped_type = typename decltype((std::type_identity<Args>{}, ...))::type;
    using null = struct null;

    // class iterator {
    //     std::unordered_map<last_key, mapped_type>::iterator current;
    // };

    multikey_map() = default;

    multikey_map(std::initializer_list<value_type> ilist) : map_(ilist) {}

    auto operator=(std::initilizer_list<value_type> ilist) {
        map_ = ilist;
    }

    multikey_map(multikey_map const& other) : map_(other.map_) {}

    auto operator=(multikey_map const& other) -> multikey_map& {
        return multikey_map(other).swap(*this);
    }

    multikey_map(multikey_map&& other) noexcept {
        std::swap(map_, other.map_);
        other.clear();
    }

    auto operator=(multikey_map&& other) noexcept -> multikey_map& {
        std::swap(map_, other.map_);
        other.clear();
        return *this;
    }

    auto clear() noexcept -> void {
        map_.clear();
    }

    [[nodiscard]] auto empty() const noexcept -> bool {
        return map_.empty();
    }

    [[nodiscard]] auto size() const noexcept -> std::size_t {
        return map_.size();
    }

    template <typename T>
    auto operator[](T const& index) -> decltype(map_[index])& {
        return map_[index];
    }

    template <typename T>
    auto operator[](T const& index) const -> decltype(map_[index]) {
        return map_[index];
    }

    // As close as we can get to MATLAB's : operator.
    // Null type means we don't care about this index i.e. search all values from
    // Lets us branch in our search
    // auto operator[](null const& n) -> decltype(map_)& {
    //     return map_.va;
    // }

    // auto operator[](null const& n) const -> decltype(map_) {
    //     return map_[index];
    // }

   private:
    auto swap(multikey_map& other) noexcept -> multikey_map& {
        map_.swap(other.map_);
        return *this;
    }
};

}  // namespace xtl