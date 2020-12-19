/**
** \file Lifetime.hpp
**
** \author Phantomas <phantomas@phantomas.xyz>
** \date Created on: 2020-10-18 23:20
** \date Last update: 2020-10-18 23:20
** \copyright GNU Lesser Public Licence v3
*/

#ifndef utils_Lifetimes_hpp__
#define utils_Lifetimes_hpp__

namespace clonixin::type_desc {
    enum class Lifetime {
        Singleton,
        Transient
    };
}

#endif

