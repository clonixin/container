/**
** \file IBuilder.hpp
**
** \author Phantomas <phantomas@phantomas.xyz>
** \date Created on: 2020-05-08 16:05
** \date Last update: 2020-05-08 22:56
*/

#ifndef builders_IBuilder_hpp__
#define builders_IBuilder_hpp__

#include <any>
#include <typeindex>

#include "containers/ContainerFwd.hpp"

namespace clonixin::builders {
    class IBuilder {
        public:
            virtual ~IBuilder() {}

            virtual std::any build(Container const &container) const = 0;

            virtual std::type_index getTypeIndex() const noexcept = 0;
    };
}

#endif
