/**
** \file ContainerFwd.hpp
**
** \author Phantomas <phantomas@phantomas.xyz>
** \date Created on: 2020-05-08 20:37
** \date Last update: 2020-05-08 22:33
*/

#ifndef containers_ContainerFwd_hpp__
#define containers_ContainerFwd_hpp__

#ifdef containers_Container_hpp__
#undef containers_Container_hpp__
#endif

#define __CONTAINER_FWD_ONLY
namespace clonixin::builders {
    class IBuilder;
}
#include "containers/Container.hpp"
#undef __CONTAINER_FWD_ONLY

#endif
