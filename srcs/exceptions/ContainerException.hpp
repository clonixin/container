/**
** \file ContainerException.hpp
**
** \author Phantomas <phantomas@phantomas.xyz>
** \date Created on: 2020-06-02 22:36
** \date Last update: 2020-06-08 15:27
** \copyright GNU Lesser Public Licence v3
*/

#ifndef exceptions_ContainerException_hpp__
#define exceptions_ContainerException_hpp__

#include <array>
#include <string>

#include "./GenericException.hpp"

namespace clonixin::exceptions {
    /**
    ** \brief Container error component.
    */
    inline constexpr char CONTAINER_ERROR[] = "Container Error";

    /**
    ** \brief Describe the kinf of error encountered by the container.
    */
    enum struct ContainerError : size_t {
        UNKNOWN = 0,
        BadLifetime,
        TypeNotFound,
        DuplicateType,
        LAST
    };

    using namespace std::literals;

    /**
    ** \brief Array of std::string that describe the actual error. These are
    ** then concatenated with types information if needed, and are used as the
    ** explanatory strings for the container exceptions.
    */
    inline static std::array<std::string, (size_t)ContainerError::LAST> const CONTAINER_ERROR_DESC = {
        "Unknown Container Error"s,
        "Unexpected lifetime found for type : "s,
        "Could not find builder nor factory for type : "s,
        "Another instance or builder found for type : "s
    };

    /**
    ** \brief Convenience type to create exceptions for the container, without
    ** having to specify the common parameter.
    **
    ** \tparam err ContainerError value representing the type of error. This
    ** parameter enable a user to catch specifically one kind of container error, if needed.
    */
    template <ContainerError err>
    using ContainerException = GenericException<err, CONTAINER_ERROR>;
}

#endif
