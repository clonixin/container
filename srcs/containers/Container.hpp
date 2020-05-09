/**
** \file Container.hpp
**
** \author Phantomas <phantomas@phantomas.xyz>
** \date Created on: 2020-05-08 16:10
** \date Last update: 2020-05-09 18:22
*/

#ifndef containers_Container_hpp__
#define containers_Container_hpp__

#include <any>
#include <unordered_map>
#include <memory>
#include <typeindex>

#ifndef containers_ContainerFwd_hpp__
#include "builders/IBuilder.hpp"
#include "builders/GenericBuilder.hpp"
#include "builders/AbstractBuilder.hpp"
#endif

namespace clonixin {
#ifndef __CONTAINER_DECLARED
#define __CONTAINER_DECLARED

    class Container {
        private:
            enum struct Lifetime { Singleton, Transient };
        public:
            virtual Container &addClass(std::unique_ptr<builders::IBuilder> &&builder);
            virtual Container &addSingleton(std::unique_ptr<builders::IBuilder> &&builder);

            std::any getInstance(std::type_index t) const;
        private:
            mutable std::unordered_map<std::type_index, std::any> _singleton_map;
            std::unordered_map<std::type_index, std::unique_ptr<builders::IBuilder>> _builder_map;

            std::unordered_map<std::type_index, Lifetime> _life_map;
    };
#endif

#ifndef __CONTAINER_FWD_ONLY
    Container & Container::addClass(std::unique_ptr<builders::IBuilder> &&builder) {
        auto ti= builder->getTypeIndex();

        if (_life_map.find(ti) != _life_map.end())
            ;// throw, maybe ?
        _builder_map[ti] = std::move(builder);
        _life_map[ti] = Lifetime::Transient;

        return *this;
    }

    Container & Container::addSingleton(std::unique_ptr<builders::IBuilder> &&builder) {
        auto ti= builder->getTypeIndex();

        if (_life_map.find(builder->getTypeIndex()) != _life_map.end())
            ;// throw, maybe ?
        _builder_map[ti] = std::move(builder);
        _life_map[ti] = Lifetime::Singleton;

        return *this;
    }

    std::any Container::getInstance(std::type_index t) const {
        using namespace std::literals;
        if (_life_map.find(t) == _life_map.end())
            throw std::runtime_error("Could not find type : "s + t.name());

        Lifetime l = _life_map.at(t);
        switch (l) {
            case Lifetime::Transient:
                return _builder_map.at(t)->build(*this);
            case Lifetime::Singleton:
                if (_singleton_map.find(t) == _singleton_map.end())
                    _singleton_map[t] = _builder_map.at(t)->build(*this);
                return _singleton_map.at(t);
            default:
                throw std::runtime_error("Unexpected lifetime found for type : "s + t.name());
        }
    }

#endif
}

#endif
