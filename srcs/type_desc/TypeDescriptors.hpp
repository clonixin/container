/**
** \file TypeDescriptors.hpp
**
** \author Phantomas <phantomas@phantomas.xyz>
** \date Created on: 2020-05-08 23:29
** \date Last update: 2020-05-18 16:48
** \copyright GNU Lesser Public Licence v3
*/

#ifndef utils_TypeDescriptors_hpp__
#define utils_TypeDescriptors_hpp__

#include <type_traits>

#include "type_desc/Lifetime.hpp"

namespace clonixin::type_desc {
    namespace _internals {
        /**
        ** \brief Meta-programming structure to wrap a type.
        **
        ** Fallback implementation. It use static_asserts to provide a better
        ** understanding on why the types are not valid.
        **
        ** \tparam lifetime An enum value describing the lifetime this type will have..
        ** \tparam B Base type to be declared in the container
        ** \tparam T Type that will be constructed as an implementation for B.
        ** \tparam _ Helper to trigger SFINAE
        */
        template <Lifetime lt, class B, class T =  B, typename = std::void_t<>>
        struct __type_desc {
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

        /**
        ** \brief Meta-programming structure to wrap type.
        **
        ** Polymorphic type specialization. It's used to detect polymorphic
        ** types, enabling the container to access both the base (declared)
        ** type, and he concrete type implementing it.
        **
        ** \tparam lifetime An enum value describing the lifetime this type will have..
        ** \tparam B Base type to be declared in the container
        ** \tparam T Type that will be constructed as an implementation for B.
        */
        template <Lifetime _lifetime, class B, class T>
        struct __type_desc<_lifetime, B, T, std::void_t<
            std::enable_if_t<!std::is_same_v<B, T> && !std::is_abstract_v<T> && std::is_base_of_v<B, T>>
            >> {
                using base = B;
                using type = T;

                /**
                ** \brief Boolean set to true if B is to be defined as a
                ** singleton type.
                */
                static Lifetime constexpr lifetime = _lifetime;
                /**
                ** \brief Boolean always set to true. Help leverage SFINAE
                ** in the container.
                */
                static bool constexpr is_polymorph = true;
            };

        /**
        ** \brief Meta-programming structure to wrap type.
        **
        ** Non-Polymorphic type specialization. It's used to detect polymorphic
        ** types, enabling the container to access both the base (declared)
        ** type, and he concrete type implementing it.
        **
        ** \tparam singleton Currently, a boolean to switch between transient
        ** and singleton types. This will surely be changed to an enum later.
        ** \tparam B Base type to be declared in the container
        ** \tparam T Type that will be constructed as an implementation for B.
        */
        template <Lifetime _lifetime, class T>
        struct __type_desc<_lifetime, T, T, std::void_t<
            std::enable_if_t<!std::is_abstract_v<T>>
        >> {
            using type = T;

            /**
            ** \brief Boolean set to true if B is to be defined as a
            ** singleton type.
            */
            static Lifetime constexpr lifetime = _lifetime;

            /**
            ** \brief Boolean always set to false. Help leverage SFINAE
            ** in the container.
            */
            static bool constexpr is_polymorph = false;
        };
    }

    /**
    ** \brief Transient type descriptor.
    **
    ** Using this type descriptor will register the type B as transient in the IoC Container.
    ** This means that the type will be recreated each time it's needed.
    **
    ** \tparam B The type to register.
    ** \tparam T If used, and different that B, the type that'll be used as an
    ** implementation of B.
    */
    template <class B, class T = B>
    struct Transient : public _internals::__type_desc<Lifetime::Transient, B, T> {};

    /**
    ** \brief Singleton type descriptor.
    **
    ** Using this type descriptor will register the type B as a singleton in the IoC Container.
    ** This means that the type will be constructed only once, hence will be shared by all objects.
    **
    ** \tparam B The type to register.
    ** \tparam T If used, and different that B, the type that'll be used as an
    ** implementation of B.
    */
    template <class B, class T = B>
    struct Singleton : public _internals::__type_desc<Lifetime::Singleton, B, T> {};
}

#endif
