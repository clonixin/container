/**
** \file type_traits.hpp
**
** \author Phantomas <phantomas@phantomas.xyz>
** \date Created on: 2021-06-09 00:05
** \date Last update: 2021-06-09 00:14
*/

#ifndef utils_type_traits_hpp__
#define utils_type_traits_hpp__

#include <type_traits>

namespace clonixin::type_traits {
    template <typename T, typename U, typename ... Ts>
    struct is_one_of : std::disjunction<std::is_same<T, U>, std::is_same<T, Ts>...> {};

    template <typename... Ts>
    static constexpr bool is_one_of_v = is_one_of<Ts...>::value;
}

#endif /* end of include guard: utils_type_traits_hpp__ */
