/**
** \file TypeDescriptors.hpp
**
** \author Phantomas <phantomas@phantomas.xyz>
** \date Created on: 2020-05-08 23:29
** \date Last update: 2020-05-09 04:37
*/

#ifndef utils_TypeDescriptors_hpp__
#define utils_TypeDescriptors_hpp__

#include <type_traits>

namespace clonixin::utils {
    struct TypeDescriptor {
        static bool constexpr is_singleton = false;
        static bool constexpr is_polymorph = false;
    };

    template <bool singleton, class B, class T =  B, typename = std::void_t<>>//, bool valid = false>
    struct __type_desc: public TypeDescriptor {
        static_assert(
            !std::disjunction_v<
                std::conjunction<
                    std::negation<std::is_same<B, T>>,
                    std::negation<std::is_base_of<B, T>>
                >
            >,
            "Types B and T are different, and B is not a base of T."
        );

        static_assert(!std::is_same_v<B, T> && !std::is_abstract_v<T>, "Type T should not be abstract.");

        using base = B;
        using type = T;
    };

    template <bool singleton, class B, class T>
    struct __type_desc<singleton, B, T, std::void_t<
        std::enable_if_t<!std::is_same_v<B, T> && !std::is_abstract_v<T> && std::is_base_of_v<B, T>>
    >> {
        using base = B;
        using type = T;

        static bool constexpr is_singleton = singleton;
        static bool constexpr is_polymorph = true;
    };

    template <bool singleton, class T>
    struct __type_desc<singleton, T, T, std::void_t<
        std::enable_if_t<!std::is_abstract_v<T>>
    >> {
        using type = T;

        static bool constexpr is_singleton = singleton;
        static bool constexpr is_polymorph = false;
    };

    template <class B, class T = B>
    using Class = __type_desc<false, B, T>;

    template <class B, class T = B>
    using Singleton = __type_desc<true, B, T>;
}

#endif
