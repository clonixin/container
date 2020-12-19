/**
** \file LambdaBuilder.hpp
**
** \author Phantomas <phantomas@phantomas.xyz>
** \date Created on: 2020-05-08 22:48
** \date Last update: 2020-06-08 22:02
** \copyright GNU Lesser Public Licence v3
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
    /**
    ** \brief Lambda builder wrapper.
    **
    ** This class allow for the usage of a custom builder. It wrap a function that will be called
    ** whenever a value of the type represented by type_index will be created.
    **
    ** The function must accept a boolean as it's second argument. It's be used to discriminate between
    */
    template <class Function>
    class LambdaBuilder : public IBuilder {
        public:
        /**
        ** \brief Build a new LambdaBuilder.
        **
        ** This contructor use a static asser to ensure the function is compatible, then store it.
        */
        LambdaBuilder(std::type_index idx, Function fun)
            : _index(idx), _lambda(fun)
        {
            static_assert(
                std::is_invocable_r_v<std::any, Function, Container, bool>,
                "Function type should be invocable with Container and bool as its args."
            );
        }
        /**
        ** \brief LambdaBuilder destructor.
        */
        virtual ~LambdaBuilder() noexcept {}

        /**
        ** \brief Call the wrapped function to build the object, and return a shared_ptr to it.
        */
        std::any buildPtr(Container const &c) const override { return _lambda(c, false); }
        /**
        ** \brief Call the wrapped function to build the object and return it as a value inside the std::any
        */
        std::any buildVal(Container const &c) const override { return _lambda(c, true); }

        std::type_index getTypeIndex() const noexcept override { return _index; }

        private:
            std::type_index const _index;
            std::function<std::any (Container const &, bool)> _lambda;
    };
}

#endif
