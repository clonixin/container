/**
** \file LambdaBuilder.hpp
**
** \author Phantomas <phantomas@phantomas.xyz>
** \date Created on: 2020-05-08 22:48
** \date Last update: 2020-05-08 22:56
*/

#ifndef builders_LambdaBuilder_hpp__
#define builders_LambdaBuilder_hpp__

#include <any>
#include <functional>
#include <memory>
#include <typeindex>

#include "containers/ContainerFwd.hpp"
#include "builders/IBuilder.hpp"

namespace clonixin::builders {
    class LambdaBuilder : public IBuilder {
        LambdaBuilder(std::type_index, std::function<std::any (Container const &)>);
        virtual ~LambdaBuilder() {}

        std::any build(Container const &c) const override { return _lambda(c); }

        std::type_index getTypeIndex() const noexcept override { return _index; }

        private:
            std::type_index const _index;
            std::function<std::any (Container const &)> _lambda;
    };
}

#endif
