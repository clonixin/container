#ifndef clx_container_hpp__
#define clx_container_hpp__

#include <containers/Container.hpp>

/**
** \brief Base Clonixin Namespace
*/
namespace clonixin {
    /**
    ** \brief Default instance, if one want to use it as a Singleton.
    */
    inline clonixin::Container &defaultContainer() {
        static clonixin::Container c;
        return c;
    }
}

#endif
