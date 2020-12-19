/**
** \file AbstractBuilder.hpp
**
** \author Phantomas <phantomas@phantomas.xyz>
** \date Created on: 2020-05-08 22:35
** \date Last update: 2020-06-08 15:41
*/

#ifndef builders_AbstractBuilder_hpp__
#define builders_AbstractBuilder_hpp__

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
    ** \brief Automatic builder, for polymorphic types.
    **
    ** This is the default builder when registering a polymorphic type to
    ** the container, using the generic functions.
    **
    ** \tparam Base Type of the base class. This is the type that will be
    ** requested if and when needed.
    ** \tparam T Type of the instance that'll be built, which should be a valid
    ** implementation of Base.
    ** \tparam As... Variadic pareameters, containing either a type, or a value
    ** holding type.
    **
    ** \pre T must be a valid implementation of Base.
    */
    template <class Base, class T, typename... As>
    class AbstractBuilder: public IBuilder {
        public:
            /**
            ** \brief AbstractBuilder destructor.
            */
            virtual ~AbstractBuilder() {}

            [[nodiscard]]
            std::any buildPtr(Container const &container) const override;
            [[nodiscard]]
            std::any buildVal(Container const &container) const override;
            std::type_index getTypeIndex() const noexcept override;
    };

    /**
    ** \brief Build an instance of type T, using As types, and return it as a
    ** Base.
    **
    ** This function builds an instance of T, store it as a Base in a
    ** std::shared_ptr, then return it as an std::any.
    **
    ** What it actually does is calling @::new T, and pass each As as a
    ** parameter, either by requesting an instance to the container, or by
    ** unwrapping the value, if As is a value holding type. It then store the
    ** pointer to the newly created instance in a std::shared_ptr<Base>, then
    ** wrap it in a std::any upon completion.
    **
    ** \tparam Base Type of the base class. This is the type that will be
    ** requested if and when needed.
    ** \tparam T Type of the instance that'll be built, which should be a valid
    ** implementation of Base.
    ** \tparam As... Variadic parameters, containing either a type, or a value
    ** holding type.
    **
    ** \pre T must be a valid implementation of Base.
    **
    ** \param container Clonixin IoC container.
    **
    ** \return This function returns a newly created instance, inside a
    ** std::shared_ptr<Base>, wrapped in a std::any.
    */
    template <class B, class T, typename... As>
    inline std::any AbstractBuilder<B, T, As...>::buildPtr(Container const &container) const {
        static_assert(std::is_base_of_v<B, T>, "Base is not base class of T.");
        return std::shared_ptr<B>( ::new T(utils::value::_internals::__value_unwrapper<As>::value(container)...));
    }

    /**
    ** \brief Build an instance of type T, using As types, and return it as a
    ** Base.
    **
    ** This function builds an instance of T, store it as a Base in a
    ** std::shared_ptr, then return it as an std::any.
    **
    ** What it actually does is calling @::new T, and pass each As as a
    ** parameter, either by requesting an instance to the container, or by
    ** unwrapping the value, if As is a value holding type. It then store the
    ** pointer to the newly created instance in a std::shared_ptr<Base>, then
    ** wrap it in a std::any upon completion.
    **
    ** \tparam Base Type of the base class. This is the type that will be
    ** requested if and when needed.
    ** \tparam T Type of the instance that'll be built, which should be a valid
    ** implementation of Base.
    ** \tparam As... Variadic parameters, containing either a type, or a value
    ** holding type.
    **
    ** \pre T must be a valid implementation of Base.
    **
    ** \param container Clonixin IoC container.
    **
    ** \return This function returns a newly created instance, inside a
    ** std::shared_ptr<Base>, wrapped in a std::any.
    */
    template <class B, class T, typename... As>
    inline std::any AbstractBuilder<B, T, As...>::buildVal(Container const &container) const {
        using Error = exceptions::BuilderError;
        throw exceptions::BuilderException<Error::AbstractRvalue>(
                exceptions::BUILDER_ERROR_DESC[(size_t)Error::AbstractRvalue] + typeid(B).name(),
                __FILE__, __LINE__
                );
    }

    /**
    ** \brief Get a std::type_index describing the type to be built.
    **
    ** This function is equivalent to calling typeid(Base).
    **
    ** \tparam Base Type of the base class. This is the type that will be
    ** requested if and when needed.
    ** \tparam T Type of the instance that'll be built, which should be a valid
    ** implementation of Base.
    ** \tparam As... Variadic pareameters, containing either a type, or a value
    ** holding type.
    **
    ** \pre T must be a valid implementation of Base.
    **
    ** \return The std::type_index of the instance that'll be built.
    */
    template <class Base, class T, typename... As>
    inline std::type_index AbstractBuilder<Base, T, As...>::getTypeIndex() const noexcept {
        return typeid(Base);
    }
}

#endif
