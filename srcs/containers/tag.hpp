/**
** \file tag.hpp
**
** \author Phantomas <phantomas@phantomas.xyz>
** \date Created on: 2021-06-08 23:31
** \date Last update: 2021-06-12 23:17
*/

#ifndef containers_tag_hpp__
#define containers_tag_hpp__

namespace clonixin::tag {
    inline namespace container {
        /**
        ** \brief Container's disambiguation helpers
        ** Empty type to disambiguate retrieval via ptr or r-value reference in Container functions.
        */
        struct ptr_t{};

        /**
        ** \brief Container's disambiguation helpers
        ** Empty type to disambiguate retrieval via ptr or r-value reference in Container functions.
        */
        struct rref_t{};

        /**
        ** \brief Container's disambiguation instance
        ** Empty type to disambiguate retrieval via ptr or r-value reference in Container functions.
        */
        inline constexpr ptr_t ptr{};

        /**
        ** \brief Container's disambiguation instance
        ** Empty type to disambiguate retrieval via ptr or r-value reference in Container functions.
        */
        inline constexpr rref_t rref{};

        inline namespace duplicate {
            /**
            ** \brief Control whether add* function should throw if a type has already been registered.
            */
            struct once_t{};

            /**
            ** \brief Control whether add* function should override duplicate types. Default handling.
            */
            struct override_t{};

            /**
            ** \brief Control whether add* function should silently ignore duplicate types.
            */
            struct ignore_t{};
            inline constexpr once_t once{};
            inline constexpr override_t over{};
            inline constexpr ignore_t ignore{};
        }
    }
}

#endif
