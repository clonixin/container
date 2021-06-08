/**
** \file Container.hpp
**
** \author Phantomas <phantomas@phantomas.xyz>
** \date Created on: 2020-05-08 16:10
** \date Last update: 2021-01-17 13:36
** \copyright GNU Lesser Public Licence v3
*/

#ifndef containers_Container_hpp__
#define containers_Container_hpp__

#include <any>
#include <unordered_map>
#include <memory>
#include <stdexcept>
#include <string>
#include <typeindex>
#include <type_traits>

#include "./tag.hpp"

#ifndef containers_ContainerFwd_hpp__
#include "builders/IBuilder.hpp"
#include "builders/GenericBuilder.hpp"
#include "builders/AbstractBuilder.hpp"
#endif

#include "exceptions/ContainerException.hpp"
#include "type_desc.hpp"

namespace clonixin {
#ifndef __CONTAINER_DECLARED
#define __CONTAINER_DECLARED

    /**
    ** \brief Clonixin's DI container.
    **
    ** This class is the IoC Container implementation of Clonixin.
    ** It's charged to manage class creation, and lifetime.
    ** It works by either storing or building at compile-time classes factories
    ** and then either building or returning a stored instance when needed, in
    ** case if singleton.
    **
    ** This also support standalone instances, that act as Singleton.
    **
    ** \par Lifetime:
    ** The lifetime of a type represent how often a new instance will be created.
    ** At the moment, there are only two possible values:
    **  - Singleton: There will be only one instance of this type.
    **  - Transient: A new instance will be created each time it's requested.
    ** Some more lifetime are planned, but not developed yet. These include:
    **  - Scoped: A type that'll act as a Singleton in a given scope. What that
    **  means is that there will be a way to declare a scope, and as long as
    **  it's valid, all variable created in the same C++ scope, or in subscope
    **  will use the same instance.
    **  - Request: A type that act as a singleton, but only during a given
    **  request. This lifetime will enable to create one instance, that'll be
    **  reused when building all necessary instances. For example, you'll be
    **  able to instantiate a single logger, for all build dependencies of a
    **  given class.
    **
    ** \par Usage:
    ** Upon instantiation, the container is empty. The user must register types
    ** by adding them, adding standalone instances, or providing a custom
    ** builder class.
    ** You then call Container::getInstance<T>() to retrieve an instance of type
    ** T.
    */
    class Container {
        public:
            virtual ~Container() {}
            virtual Container &addTransient(std::unique_ptr<builders::IBuilder> &&builder);
            virtual Container &addSingleton(std::unique_ptr<builders::IBuilder> &&builder);

            template <class T> Container &addInstance(std::unique_ptr<T> &&obj);

            template <class TypeDesc, typename... As> std::enable_if_t<TypeDesc::is_polymorph, Container &> addType();
            template <class TypeDesc, typename... As> std::enable_if_t<!TypeDesc::is_polymorph, Container &> addType();

            virtual std::any getInstance(tag::container::ptr_t, std::type_index t) const;
            virtual std::any getInstance(tag::container::rref_t, std::type_index t) const;

            template <class T> std::enable_if_t<std::negation_v<std::is_rvalue_reference<T>>, std::shared_ptr<T>> getInstance() const;
            template <class T> std::enable_if_t<std::is_rvalue_reference_v<T>, T &&> getInstance() const;

        private:
            virtual std::any _typeNotFound(tag::container::ptr_t, std::type_index) const;
            virtual std::any _typeNotFound(tag::container::rref_t, std::type_index) const;

        private:
            mutable std::unordered_map<std::type_index, std::any> _singleton_map;
            std::unordered_map<std::type_index, std::unique_ptr<builders::IBuilder>> _builder_map;

            std::unordered_map<std::type_index, type_desc::Lifetime> _life_map;
    };
#endif

#ifndef __CONTAINER_FWD_ONLY

#include "utils/ValueWrapper.hpp"

    /**
    ** \brief Register a class builder to the container.
    **
    ** \param builder an rvalue reference to a std::unique_ptr<IBuilder>.
    **
    ** \return The current Container instance.
    */
    inline Container & Container::addTransient(std::unique_ptr<builders::IBuilder> &&builder) {
        auto ti = builder->getTypeIndex();

        if (_life_map.find(ti) != _life_map.end())
            ;// throw, maybe ?
        _builder_map[ti] = std::move(builder);
        _life_map[ti] = type_desc::Lifetime::Transient;

        return *this;
    }

    /**
    ** \brief Register a singleton builder to the container
    **
    ** \param builder an rvalue reference to a std::unique_ptr<IBuilder>.
    **
    ** \return The current Container instance.
    */
    inline Container & Container::addSingleton(std::unique_ptr<builders::IBuilder> &&builder) {
        auto ti= builder->getTypeIndex();

        if (_life_map.find(builder->getTypeIndex()) != _life_map.end())
            ;// throw, maybe ?
        _builder_map[ti] = std::move(builder);
        _life_map[ti] = type_desc::Lifetime::Singleton;

        return *this;
    }

    /**
    ** \brief Register a singleton instance in the container.
    **
    ** \param obj The instance to register.
    **
    ** \tparam T Type of the object.
    **
    ** \return The current Container instance.
    */
    template <class T>
    inline Container & Container::addInstance(std::unique_ptr<T> &&obj) {
        std::type_index idx = typeid(T);

        _singleton_map[idx] = std::shared_ptr(std::forward<std::unique_ptr<T> &&>(obj));
        _life_map[idx] = type_desc::Lifetime::Singleton;

        return *this;
    }

    /**
    ** \brief Register a non polymorphic type to the container.
    **
    ** Calling this function create a builder, that will create every needed
    ** instance to create the instance describe by the TypeDesc argument.
    ** A TypeDesc is needed to specify the type and lifetime, as well as to
    ** discriminate between polymorphic and non polymorphic type registration.
    **
    ** Variadic template arguments are there to specify the type of each
    ** parameter. There must be a matching constructor in the type represented
    ** by TypeDesc.
    ** It's also possible to specify some values, either by passing a Direct<T>
    ** or an Indirect<T> type, that'll wrap the value.
    **
    ** \tparam TypeDesc A Type-descriptor type.
    ** \tparam As Types of the class' constructor arguments, that will be built
    ** on the fly or retrieved, as well as value wrapping types of the
    ** argument that cannot be built that way (strings, algebraic types, etc.)
    **
    ** \return The current Container instance.
    */
    template <class TypeDesc, typename... As>
    inline std::enable_if_t<!TypeDesc::is_polymorph, Container &> Container::addType() {
        using T = typename TypeDesc::type;

        {
            using clonixin::utils::value::_internals::__value_unwrapper;
            static_assert(std::is_constructible_v<T, typename __value_unwrapper<As>::type...>,
                    "Cannot construct type.");
        }

        std::type_index idx = typeid(T);

        _builder_map[idx] = std::make_unique<builders::GenericBuilder<T, As...>>();

        _life_map[idx] = TypeDesc::lifetime;

        return *this;
    }

    /**
    ** \brief Register a polymorphic type to the container.
    **
    ** Calling this function create a builder, that will create every needed
    ** instance to create the instance describe by the TypeDesc argument.
    ** A TypeDesc is needed to specify the type and lifetime, as well as to
    ** discriminate between polymorphic and non polymorphic type registration.
    **
    ** This version is called when the TypeDesc represent a polymorphic type.
    ** more precisely, when the Descriptor contains a base types, as well as
    ** an implementation type for it.
    **
    ** Variadic template arguments are there to specify the type of each
    ** parameter. There must be a matching constructor in the type represented
    ** by TypeDesc.
    ** It's also possible to specify some values, either by passing a Direct<T>
    ** or an Indirect<T> type, that'll wrap the value.
    **
    ** \tparam TypeDesc A Type-descriptor type.
    ** \tparam As Types of the class' constructor arguments, that will be built
    ** on the fly or retrieved, as well as value wrapping types of the
    ** argument that cannot be built that way (strings, algebraic types, etc.)
    **
    ** \return The current Container instance.
    */
    template <class TypeDesc, typename... As>
    inline std::enable_if_t<TypeDesc::is_polymorph, Container &> Container::addType() {
        using T = typename TypeDesc::type;
        using B = typename TypeDesc::base;

        {
            using clonixin::utils::value::_internals::__value_unwrapper;
            static_assert(std::is_constructible_v<T, typename __value_unwrapper<As>::type...>,
                    "Cannot construct type.");
        }

        std::type_index idx = typeid(B);

        _builder_map[idx] = std::make_unique<builders::AbstractBuilder<B, T, As...>>();

        _life_map[idx] = TypeDesc::lifetime;

        return *this;
    }

    /**
    ** \brief Get a given instance wrapped in a shared_ptr.
    **
    ** This function build or retrieved a given instance, building every
    ** needed types in the process, then return a std::shared_ptr to the
    ** instances, by wrapping it in a std::any.
    **
    ** \param t A std::type_index, that represent the type to be retrieved.
    **
    ** \return If the instance could be built, it's returned. Otherwise a
    ** std::runtime_error will be thrown.
    **
    ** \throw exceptions::ContainerException<exceptions::ContainerError::TypeNotFound>
    ** Thrown if the type has not been registered.
    ** \throw exceptions::ContainerException<exceptions::ContainerError::BadLifetime>
    ** Thrown if an invalid lifetime is found.
    **
    */
    inline std::any Container::getInstance(tag::container::ptr_t, std::type_index t) const {
        using type_desc::Lifetime;
        using Error = clonixin::exceptions::ContainerError;
        if (_life_map.find(t) == _life_map.end())
            _typeNotFound(tag::container::ptr, t);

        Lifetime l = _life_map.at(t);
        switch (l) {
            case Lifetime::Transient:
                return _builder_map.at(t)->buildPtr(*this);
            case Lifetime::Singleton:
                if (_singleton_map.find(t) == _singleton_map.end())
                    _singleton_map[t] = _builder_map.at(t)->buildPtr(*this);
                return _singleton_map.at(t);
            default: //GCOV_EXCL_START
            throw exceptions::ContainerException<Error::BadLifetime>(
                exceptions::CONTAINER_ERROR_DESC[(size_t)Error::BadLifetime] + t.name(),
                __FILE__, __LINE__
            );
        } //GCOV_EXCL_STOP
    }

    /**
    ** \brief Get a given instance, as a rvalue..
    **
    ** This function build or retrieved a given instance, building every
    ** needed types in the process, then return it's value wrapped in a std::any.
    **
    ** \param t A std::type_index, that represent the type to be retrieved.
    **
    ** \return If the instance could be built, it's returned. Otherwise a
    ** std::runtime_error will be thrown.
    **
    ** \throw exceptions::ContainerException<exceptions::ContainerError::TypeNotFound>
    ** Thrown if the type has not been registered.
    ** \throw exceptions::ContainerException<exceptions::ContainerError::BadLifetime>
    ** Thrown if an invalid lifetime is found.
    **
    */
    inline std::any Container::getInstance(tag::container::rref_t, std::type_index t) const {
        using type_desc::Lifetime;
        using Error = clonixin::exceptions::ContainerError;
        if (_life_map.find(t) == _life_map.end())
            _typeNotFound(tag::container::rref, t);

        Lifetime l = _life_map.at(t);
        switch (l) {
            case Lifetime::Transient:
                return _builder_map.at(t)->buildVal(*this);
            case Lifetime::Singleton:
                throw exceptions::ContainerException<Error::BadLifetime>(
                        exceptions::CONTAINER_ERROR_DESC[(size_t)Error::BadLifetime] + t.name() +
                        ". Cannot return a singleton using move semantics.",
                        __FILE__, __LINE__
                        );
            default: //GCOV_EXCL_START
            throw exceptions::ContainerException<Error::BadLifetime>(
                exceptions::CONTAINER_ERROR_DESC[(size_t)Error::BadLifetime] + t.name(),
                __FILE__, __LINE__
            );
        } //GCOV_EXCL_STOP
    }

    /**
    ** \brief Get a given instance as a shared_ptr.
    **
    ** This function build or retrieved a given instance, building every
    ** needed types in the process, then return a std::shared_ptr to the
    ** instances.
    **
    ** This function build or retrieved a given instance, building every
    ** needed types in the process, then return the instances, by wrapping it
    **
    ** \tparam T The type of the instance to be returned.
    **
    ** \return If the instance could be built, it's returned. Otherwise a
    ** std::runtime_error will be thrown.
    **
    ** \throw std::runtime_error If the type is not found, or if it's lifetime is an improper
    ** value, the function throws an std::runtime_error.
    */
    template <typename T>
    inline std::enable_if_t<std::negation_v<std::is_rvalue_reference<T>>, std::shared_ptr<T>> Container::getInstance() const {
        std::type_index idx = typeid(T);

        return std::any_cast<std::shared_ptr<T>>(getInstance(tag::container::ptr, idx));
    }

    /**
    ** \brief Get a given instance, using move semantics.
    **
    ** This function build or retrieved a given instance, building every
    ** needed types in the process, then return a std::shared_ptr to the
    ** instances.
    **
    ** This function build or retrieved a given instance, building every
    ** needed types in the process, then return the instances, by wrapping it
    **
    ** \tparam T The type of the instance to be returned.
    **
    ** \return If the instance could be built, it's returned. Otherwise a
    ** std::runtime_error will be thrown.
    **
    ** \throw std::runtime_error If the type is not found, or if it's lifetime is an improper
    ** value, the function throws an std::runtime_error.
    */
    template <class T>
    inline std::enable_if_t<std::is_rvalue_reference_v<T>, T &&> Container::getInstance() const {
        std::type_index idx = typeid(T);

        return std::any_cast<T &&>(getInstance(tag::container::rref, idx));
    }

    inline std::any Container::_typeNotFound(tag::container::ptr_t, std::type_index t) const {
        using Error = clonixin::exceptions::ContainerError;
        throw exceptions::ContainerException<Error::TypeNotFound>(
            exceptions::CONTAINER_ERROR_DESC[(size_t)Error::TypeNotFound] + t.name(),
            __FILE__, __LINE__
        );
    }

    inline std::any Container::_typeNotFound(tag::container::rref_t, std::type_index t) const {
        using Error = clonixin::exceptions::ContainerError;
        throw exceptions::ContainerException<Error::TypeNotFound>(
            exceptions::CONTAINER_ERROR_DESC[(size_t)Error::TypeNotFound] + t.name(),
            __FILE__, __LINE__
        );
    }

#endif
}

#endif
