#pragma once

namespace xtl {

template <typename... T>
class nested_initializer_list {
   private:
    template <typename... T>
    struct nested_initializer_list_imple;

    template <typename T>
    struct nested_initializer_list_imple<T> {
        using type = typename std::initializer_list<T>;
    };

    template <typename... T>
    struct nested_initializer_list_imple<T...> {
        using type = typename std::initializer_list<typename nested_initializer_list_imple<T...>::type>;
    };

    typename nested_initializer_list_imple<T...>::type list_;

   public:
    using value_type = std::tuple<const T...>;
    using reference = value_type&;
    using const_reference = const value_type&;
};

}  // namespace xtl