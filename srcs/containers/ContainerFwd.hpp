/**
** \file ContainerFwd.hpp
** \brief Container class forward declaration convenience header.
**
** \warning This file is meant to be used when forward declaration of the
** Container class is needed.
**
** \see Container
**
** \author Phantomas <phantomas@phantomas.xyz>
** \date Created on: 2020-05-08 20:37
** \date Last update: 2020-05-18 13:37
** \copyright GNU Lesser Public Licence v3
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
