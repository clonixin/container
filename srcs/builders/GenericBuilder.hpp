/**
** \file GenericBuilder.hpp
**
** \author Phantomas <phantomas@phantomas.xyz>
** \date Created on: 2020-05-08 22:21
** \date Last update: 2020-05-26 12:56
** \copyright GNU Lesser Public Licence v3
*/

#ifndef builders_GenericBuilder_hpp__
#define builders_GenericBuilder_hpp__

#include <any>
#include <memory>
#include <typeinfo>
#include <typeindex>

#include "containers/ContainerFwd.hpp"
#include "builders/IBuilder.hpp"
#include "exceptions/BuilderException.hpp"
#include "utils/ValueWrapper.hpp"

namespace clonixin::builders {
    /**
    ** \brief Automatic builder, for non polymorphic types.
    **
    ** This is the default builder when registering a non-polymorphic type to
    ** the container, using the generic functions.
    **
    ** \tparam T Type of the instance that'll be built.
    ** \tparam As... Variadic pareameters, containing either a type, or a value
    ** holding type.
    */
    template <class T, typename... As>
    class GenericBuilder : public IBuilder {
        public:
            /**
            ** \brief GenericBuilder destructor.
            */
            virtual ~GenericBuilder() {}

            [[nodiscard]]
            std::any buildPtr(Container const &container) const override;
            [[nodiscard]]
            std::any buildVal(Container const &container) const override;
            std::type_index getTypeIndex() const noexcept override;
    };

    /**
    ** \brief Build an instance of type T, using As types.
    **
    ** This function builds an instance of T, in a std::shared_ptr<T>, and
    ** return it as an std::any.
    **
    ** What it actually does is calling make_shared<T>, and pass each As as a
    ** parameter, either by requesting an instance to the container, or by
    ** unwrapping the value, if As is a value holding type.
    **
    ** \tparam T Type of the instance that'll be built.
    ** \tparam As... Variadic parameters, containing either a type, or a value
    ** holding type.
    **
    ** \param container Clonixin IoC container.
    **
    ** \return This function returns a newly created instance, inside a
    ** std::shared_ptr<T>, wrapped in a std::any.
    */
    template <class T, typename... As>
    inline std::any GenericBuilder<T, As...>::buildPtr(Container const &container) const {
        return std::make_shared<T>(utils::value::_internals::__value_unwrapper<As>::value(container)...);
    }

    namespace {
        /**
        ** \internal
        ** \brief Implementation for GenericBuilder::buildVal.
        **
        ** This function builds an instance of T, in a std::any,
        **
        ** What it actually does is calling make_any<T>, and pass each As as a
        ** parameter, either by requesting an instance to the container, or by
        ** unwrapping the value, if As is a value holding type.
        **
        ** \tparam T Type of the instance that'll be built.
        ** \tparam As... Variadic parameters, containing either a type, or a value
        ** holding type.
        **
        ** \param container Clonixin IoC container.
        **
        ** \return This function returns a newly created instance,
        ** wrapped in a std::any.
        */
        template<class T, typename... As>
        inline std::enable_if_t<std::is_move_constructible_v<T>, std::any> _buildVal(Container const &container) {
            return std::make_any<T>(utils::value::_internals::__value_unwrapper<As>::value(container)...);
        }

        /**
        ** \internal
        ** \brief Implementation for GenericBuilder::buildVal to prevent compilation error.
        **
        ** This function builds an instance of T, in a std::any,
        **
        ** What it actually does is calling make_any<T>, and pass each As as a
        ** parameter, either by requesting an instance to the container, or by
        ** unwrapping the value, if As is a value holding type.
        **
        ** \tparam T Type of the instance that'll be built.
        ** \tparam As... Variadic parameters, containing either a type, or a value
        ** holding type.
        **
        ** \param container Clonixin IoC container.
        **
        ** \return This function returns a newly created instance,
        ** wrapped in a std::any.
        */
        template<class T, typename... As>
        inline std::enable_if_t<!std::is_move_constructible_v<T>, std::any> _buildVal(Container const &container) {
            using Error = exceptions::BuilderError;
            throw exceptions::BuilderException<Error::RvalueUnsupported>(
                    exceptions::BUILDER_ERROR_DESC[(size_t)Error::RvalueUnsupported] + typeid(T).name(),
                    __FILE__, __LINE__
                    );
        }
    }

    /**
    ** \brief Build an instance of type T, using As types.
    **
    ** This function builds an instance of T, in a std::any,
    **
    ** What it actually does is calling make_any<T>, and pass each As as a
    ** parameter, either by requesting an instance to the container, or by
    ** unwrapping the value, if As is a value holding type.
    **
    ** \tparam T Type of the instance that'll be built.
    ** \tparam As... Variadic parameters, containing either a type, or a value
    ** holding type.
    **
    ** \param container Clonixin IoC container.
    **
    ** \return This function returns a newly created instance,
    ** wrapped in a std::any.
    */
    template <class T, typename... As>
    inline std::any GenericBuilder<T, As...>::buildVal(Container const &container) const {
        return _buildVal<T, As...>(container);
    }

    /**
    ** \brief Get a std::type_index describing the type to be built.
    **
    ** This function is equivalent to calling typeid(T).
    **
    ** \tparam T Type of the instance that'll be built.
    ** \tparam As... Variadic pareameters, containing either a type, or a value
    ** holding type.
    **
    ** \return The std::type_index of the instance that'll be built.
    */
    template <class T, typename... As>
    inline std::type_index GenericBuilder<T, As...>::getTypeIndex() const noexcept {
        return typeid(T);
    }
}

#endif
