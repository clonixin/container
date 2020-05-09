/**
** \file AbstractBuilder.hpp
**
** \author Phantomas <phantomas@phantomas.xyz>
** \date Created on: 2020-05-08 22:35
** \date Last update: 2020-05-08 22:56
*/

#ifndef builders_AbstractBuilder_hpp__
#define builders_AbstractBuilder_hpp__

#include <any>
#include <memory>
#include <typeinfo>
#include <typeindex>

#include "containers/ContainerFwd.hpp"
#include "builders/IBuilder.hpp"

namespace clonixin::builders {
    template <class Base, class T, typename... As>
    class AbstractBuilder: public IBuilder {
        public:
            virtual ~AbstractBuilder() {}

            std::any build(Container const &container) const override;
            std::type_index getTypeIndex() const noexcept override;
    };

    template <class B, class T, typename... As>
    std::any AbstractBuilder<B, T, As...>::build(Container const &container) const {
        static_assert(std::is_base_of_v<B, T>, "Base is note base class of T.");
        return std::shared_ptr<B>( ::new T(container.getInstance<As>()...));
    }

    template <class B, class T, typename... As>
    std::type_index AbstractBuilder<B, T, As...>::getTypeIndex() const noexcept {
        return typeid(B);
    }
}

#endif
