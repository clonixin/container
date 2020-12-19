/**
** \file BuilderException.hpp
**
** \author Phantomas <phantomas@phantomas.xyz>
** \date Created on: 2020-06-02 22:36
** \date Last update: 2020-06-08 15:27
** \copyright GNU Lesser Public Licence v3
*/

#ifndef exceptions_BuilderException_hpp__
#define exceptions_BuilderException_hpp__

#include <array>
#include <string>

#include "./GenericException.hpp"

namespace clonixin::exceptions {
    /**
    ** \brief Builder error component.
    */
    inline constexpr char BUILDER_ERROR[] = "Builder Error";

    /**
    ** \brief Describe the kind of error encountered by a builder .
    */
    enum struct BuilderError : size_t {
        UNKNOWN = 0,
        RvalueUnsupported,
        AbstractRvalue,
        LAST
    };

    namespace {
        using namespace std::literals;
    }

    /**
    ** \brief Array of std::string that describe the actual error. These are
    ** then concatenated with types information if needed, and are used as the
    ** explanatory strings for the container exceptions.
    */
    inline static std::array<std::string, (size_t)BuilderError::LAST> const BUILDER_ERROR_DESC = {
        "Unknown Builder Error"s,
        "Cannot move construct : "s,
        "Won't use move on polymorphic class : "s
    };

    /**
    ** \brief Convenience type to create exceptions for the container, without
    ** having to specify the common parameter.
    **
    ** \tparam err BuilderError value representing the type of error. This
    ** parameter enable a user to catch specifically one kind of container error, if needed.
    */
    template <BuilderError err>
    using BuilderException = GenericException<err, BUILDER_ERROR>;
}

#endif
