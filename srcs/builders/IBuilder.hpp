/**
** \file IBuilder.hpp
**
** \author Phantomas <phantomas@phantomas.xyz>
** \date Created on: 2020-05-08 16:05
** \date Last update: 2020-06-08 21:47
** \copyright GNU Lesser Public Licence v3
*/

#ifndef builders_IBuilder_hpp__
#define builders_IBuilder_hpp__

#include <any>
#include <typeindex>

#include "containers/ContainerFwd.hpp"

/**
** \brief Clonixin default Builders namespace
*/
namespace clonixin::builders {
    /**
    ** \brief Builder interface
    **
    ** This interface describe a builder, that is, a factory that'll build the
    ** instance of a class, by requesting any needed types to the Container.
    **
    ** The instance is built inside a shared_ptr, then stored in a std::any.
    */
    class IBuilder {
        public:
            /**
            ** \brief IBuilder destructor.
            */
            virtual ~IBuilder() {}

            /**
            ** \brief Build an instance of a class, and return it as a shared_ptr.
            **
            ** This function does most of the job of the builder. It's goal is
            ** to build an instance of the class, by requesting any
            ** dependencies to the IoC container.
            **
            ** \param container IoC container to which the other dependencies
            ** are requested.
            **
            ** \return This function returns a std::any, wrapping a shared_ptr.
            */
            [[nodiscard]]
            virtual std::any buildPtr(Container const &container) const = 0;

            /**
            ** \brief Build an instance of a class inside a std::any directly..
            **
            ** This function does most of the job of the builder. It's goal is
            ** to build an instance of the class, by requesting any
            ** dependencies to the IoC container.
            **
            ** \param container IoC container to which the other dependencies
            ** are requested.
            **
            ** \return This function returns a std::any, wrapping the value
            */
            [[nodiscard]]
            virtual std::any buildVal(Container const &container) const = 0;

            /**
            ** \brief Get a std::type_index describing the type to be built.
            **
            ** This function is called internally when the container needs to
            ** register the builder to it's builder container.
            **
            ** \return The std::type_index of the instance that'll be built.
            */
            virtual std::type_index getTypeIndex() const noexcept = 0;
    };
}

#endif
