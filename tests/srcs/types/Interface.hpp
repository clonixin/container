#pragma once

namespace tests::types {
    template <int discr = 0>
    class Interface {
        public:
            virtual ~Interface() {};

            virtual int getDiscr() const = 0;
    };
}
