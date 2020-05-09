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
#include <typeindex>

#ifndef containers_ContainerFwd_hpp__
#include "builders/IBuilder.hpp"
#endif

namespace clonixin {
#ifndef __CONTAINER_DECLARED
#define __CONTAINER_DECLARED

    class Container {
        public:
            std::any getInstance(std::type_index t) const;
    };
#endif
}

#endif
