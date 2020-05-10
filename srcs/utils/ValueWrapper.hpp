/**
** \file ValueWrapper.hpp
**
** \author Phantomas <phantomas@phantomas.xyz>
** \date Created on: 2020-05-08 23:33
** \date Last update: 2020-05-10 19:24
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
        template <typename T> struct __type_of_member {
            using type = T;
        };

        template <class T, typename M> struct __type_of_member<M T::*> {
            using type = M;
        };

        template <typename T>
        struct __value_wrap {
            using type = T;
        };

        template <typename T, typename = std::void_t<>>
        struct __value_unwrapper {
            using type = std::shared_ptr<T>;

            static type value(Container const &c) { return c.getInstance<T>(); }
        };

        template <typename T>
        struct __value_unwrapper<T, std::void_t<
            std::enable_if_t<std::is_base_of_v<__value_wrap<typename T::type>, T>>
        >> {
            using type = typename T::type;

            static constexpr type value(Container const &c) { return T::value; }
        };
    }

    template <auto val, typename T = decltype(val)> struct Direct: public _internals::__value_wrap<T> {
        using type = T;
        static constexpr type value = val;
    };

    template <typename T, typename R = decltype(std::declval<T>().value)> struct Indirect : public _internals::__value_wrap<R> {
        using type = R;
        static constexpr type value = T().value;
    };

#ifdef __COMP_GCC__
    namespace _internals {
        template <char... chars>
        using __litt_string = std::integer_sequence<char, chars...>;
    }

    template <typename T, T... chars>
    constexpr _internals::__litt_string<chars ...> operator ""_str() { return {}; }

    template <typename>
    struct _string_impl {};

    template <char... chars>
        struct _string_impl<_internals::__litt_string<chars...>> {
            static constexpr std::string value() { return std::string{chars...}; };
        };

    template <typename>
    struct String {};

    template <char... chars>
    struct String<_internals::__litt_string<chars...>> : public _internals::__value_wrap<std::string> {
        using type = std::string;
        static std::string value;
    };

    template<char... chars>
    std::string String<_internals::__litt_string<chars...>>::value = _string_impl<_internals::__litt_string<chars...>>::value();
#else
    namespace _internals {
        template <char... chars>
        using __litt_string = std::integer_sequence<char, chars...>;
    }

    template <typename T, T... chars>
    constexpr _internals::__litt_string<chars ...> operator ""_str() { return {}; }

    template <typename T>
        struct String {
            static_assert(!std::is_same_v<T, T>, "String value_type is unsupported by clang. Use Indirect instead.");
        };
#endif

#define X_VALUEDECLARATOR(Type, Name) template <Type val> using Name = Direct<val, Type>
    X_VALUEDECLARATOR(bool, Bool);

    X_VALUEDECLARATOR(short, Short);
    X_VALUEDECLARATOR(unsigned short, UShort);
    X_VALUEDECLARATOR(int, Int);
    X_VALUEDECLARATOR(unsigned int, UInt);
    X_VALUEDECLARATOR(long, Long);
    X_VALUEDECLARATOR(unsigned long, ULong);
    X_VALUEDECLARATOR(long long, LongLong);
    X_VALUEDECLARATOR(unsigned long long, ULongLong);

    X_VALUEDECLARATOR(char, Char);
    X_VALUEDECLARATOR(signed char, SChar);
    X_VALUEDECLARATOR(unsigned char, UChar);
    X_VALUEDECLARATOR(wchar_t, WChar);
    X_VALUEDECLARATOR(char16_t, Char16);
    X_VALUEDECLARATOR(char32_t, Char32);
#undef X_VALUEDECLARATOR
}

#endif
