/**
** \file GenericBuilder.hpp
**
** \author Phantomas <phantomas@phantomas.xyz>
** \date Created on: 2020-05-08 22:21
** \date Last update: 2020-05-10 03:13
*/

#ifndef builders_GenericBuilder_hpp__
#define builders_GenericBuilder_hpp__

#include <any>
#include <memory>
#include <typeinfo>
#include <typeindex>

#include "containers/ContainerFwd.hpp"
#include "builders/IBuilder.hpp"
#include "utils/ValueWrapper.hpp"

namespace clonixin::builders {
    template <class T, typename... As>
    class GenericBuilder : public IBuilder {
        public:
            virtual ~GenericBuilder() {}

            std::any build(Container const &container) const override;
            std::type_index getTypeIndex() const noexcept override;
    };

    template <class T, typename... As>
    std::any GenericBuilder<T, As...>::build(Container const &container) const {
        return std::make_shared<T>(utils::value::_internals::__value_unwrapper<As>::value(container)...);
    }

    template <class T, typename... As>
    std::type_index GenericBuilder<T, As...>::getTypeIndex() const noexcept {
        return typeid(T);
    }
}

#endif
