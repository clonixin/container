/**
** \file ValueWrapper.hpp
**
** \author Phantomas <phantomas@phantomas.xyz>
** \date Created on: 2020-05-08 23:33
** \date Last update: 2020-05-30 15:37
** \copyright GNU Lesser Public Licence v3
*/

#ifndef utils_ValueWrapper_hpp__
#define utils_ValueWrapper_hpp__

#if !defined(__clang__) && defined(__GNUC__)
    # define __COMP_GCC__ 1
#endif

#include <type_traits>

#include "containers/ContainerFwd.hpp"

namespace clonixin::utils::value {
    namespace _internals {
        /**
        ** \brief Meta-programming base of Direct and Indirect value holding
        ** types.
        **
        ** \tparam T A valid type.
        */
        template <typename T>
        struct __value_wrap {
            using type = T;
        };

        /**
        ** \brief Meta-programming struct that discriminate between value
        ** holding and normal types.
        **
        ** This fallback version is instantiated for non value holding types.
        **
        ** \tparam T The type that we want to unwrap, if needed.
        */
        template <typename T, typename = std::void_t<>>
        struct __value_unwrapper {
            /**
            ** \brief The unwrapped type, or std::shared_ptr<T> if it was not a
            ** value holding type.
            */
            using type = std::shared_ptr<T>;

            /**
            ** \brief A function that return the value, or use the container to
            ** build it, it T was not a value holding type.
            */
            static type value(Container const &c) { return c.getInstance<T>(); }
        };

        /**
        ** \brief Meta-programming struct that discriminate between value
        ** holding and normal types.
        **
        ** This fallback version is instantiated for non value holding types.
        **
        ** \tparam T The type that we want to unwrap, if needed.
        */
        template <typename T>
        struct __value_unwrapper<T, std::void_t<
            std::enable_if_t<std::is_rvalue_reference_v<T>, T>
        >> {
            /**
            ** \brief The unwrapped type, or std::shared_ptr<T> if it was not a
            ** value holding type.
            */
            using type = T;

            /**
            ** \brief A function that return the value, or use the container to
            ** build it, it T was not a value holding type.
            */
            static type value(Container const &c) { return c.getInstance<T>(); }
        };

        /**
        ** \brief Meta-programming struct that discriminate between value
        ** holding and normal types.
        **
        ** This specialisation is used if T is a value holding type, that has
        ** __value_wrap as a base class. It then retrieve the wrapped type and
        ** declare it with a typedef as type.
        **
        ** It's value function returns the wrapped type, instead of using the
        ** container to build it.
        **
        ** \tparam T The type that we want to unwrap, if needed.
        */
        template <typename T>
        struct __value_unwrapper<T, std::void_t<
            std::enable_if_t<std::is_base_of_v<__value_wrap<typename T::type>, T>>
        >> {
            /**
            ** \brief The unwrapped type.
            */
            using type = typename T::type;

            /**
            ** \brief A function that return the wrapped value.
            **
            */
            static constexpr type value(Container const &c) { return T::value; }
        };
    }

    /**
    ** \brief Direct value holding type.
    **
    ** This type contains the wrapped value, and represent it's type as well as
    ** it's value.
    */
    template <auto val, typename T = decltype(val)>
    struct Direct: public _internals::__value_wrap<T> {
        /**
        ** \brief Wrapped type.
        */
        using type = T;

        /**
        ** \brief Wrapped value.
        */
        static constexpr type value = val;
    };

    /**
    ** \brief Indirect value holding type.
    **
    ** This type is mainly used for types that cannot be used as templates
    ** parameters. The type T must be constructible and have a public value
    ** attribute. The actual type is deduced by using decltype on the value
    ** attribute, if the parameter is left unspecified.
    **
    ** \tparam T A wapper type that contains the value, and must be constructible.
    ** \tparam R The type of the wrapped value.
    */
    template <typename T, typename R = decltype(std::declval<T>().value)>
    struct Indirect : public _internals::__value_wrap<R> {
        /**
        ** \brief Wrapped value type.
        */
        using type = R;

        /**
        ** \brief Wrapped value.
        */
        static constexpr type value = T().value;
    };

    /**
    ** \name Direct Partial Specialization
    */
    /**@{*/
    template <bool val> using Bool = Direct<val, bool>;

    template <short val> using Short = Direct<val, short>;
    template <unsigned short val> using UShort = Direct<val, unsigned short>;
    template <int val> using Int = Direct<val, int>;
    template <unsigned int val> using UInt = Direct<val, unsigned int>;
    template <long val> using Long = Direct<val, long>;
    template <unsigned long val> using ULong = Direct<val, unsigned long>;
    template <long long val> using LongLong = Direct<val, long long>;
    template <unsigned long long val> using ULongLong = Direct<val, unsigned long long>;

    template <char val> using Char = Direct<val, char>;
    template <signed char val> using SChar = Direct<val, signed char>;
    template <unsigned char val> using UChar = Direct<val, unsigned char>;

    template <wchar_t val> using WChar = Direct<val, wchar_t>;
    template <char16_t val> using Char16 = Direct<val, char16_t>;
    template <char32_t val> using Char32 = Direct<val, char32_t>;
    /**@}*/
}

#endif
