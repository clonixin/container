#pragma once

#include "./Interface.hpp"

namespace tests::types {
        template <int discr = 0>
        class Abstract: public Interface<discr> {
            public:
                virtual ~Abstract() = 0;

                virtual int getDiscr() const override {
                    return discr;
        };
    };

    template <int discr>
    Abstract<discr>::~Abstract() {}
}
