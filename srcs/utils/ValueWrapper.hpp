/**
** \file ValueWrapper.hpp
**
** \author Phantomas <phantomas@phantomas.xyz>
** \date Created on: 2020-05-08 23:33
** \date Last update: 2020-05-10 03:38
*/

#ifndef utils_ValueWrapper_hpp__
#define utils_ValueWrapper_hpp__

#include <type_traits>

#include "containers/ContainerFwd.hpp"

namespace clonixin::utils::value {
    namespace _internals {
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
            //std::enable_if_t<std::is_fundamental_v<decltype(T::value(std::declval<Container const &>()))>>
        >> {
            using type = typename T::type;

            static type value(Container const &c) { return T::value(c); }

        };

        template <typename T, T Val> struct __value_type: public _internals::__value_wrap<decltype(Val)> {
            using type = typename _internals::__value_wrap<decltype(Val)>::type;
            static constexpr type value(Container const &c) { return Val; }
        };

        template <char... chars>
        using __litt_string = std::integer_sequence<char, chars...>;
    }

    template <auto T>
    using Value = _internals::__value_type<decltype(T), T>;

    template <typename T, T... chars>
    constexpr _internals::__litt_string<chars ...> operator ""_str() { return {}; }

    template <typename>
    struct String {};

    template <char... chars>
    struct String<_internals::__litt_string<chars...>> : public _internals::__value_wrap<std::string> {
        using type = std::string;
        static std::string value(Container const &c) { return {chars...}; }
    };

#define X_VALUEDECLARATOR(Type, Name) template <Type val> using Name = _internals::__value_type<Type, val>
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
