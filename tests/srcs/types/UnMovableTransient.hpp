#pragma once
#include <memory>
#include <tuple>
#include <utility>

#include <utils/Counter.hpp>

namespace tests::types {
    template <class... As>
    class UnMovableTransient: public utils::Counter<UnMovableTransient<As...>> {
        using Counter = utils::Counter<UnMovableTransient<As...>>;
        public:
            UnMovableTransient(As... args) : Counter(), _content(std::move(args)...) {}
            virtual ~UnMovableTransient() {}

            UnMovableTransient(UnMovableTransient const &oth): Counter(std::forward<Counter const>(oth)), _content(oth._content) {}
            UnMovableTransient(UnMovableTransient &&) = delete;

            UnMovableTransient & operator=(UnMovableTransient const &rhs) {
                if (std::addressof(*this) != std::addressof(rhs))
                    _content = rhs._content;
                return *this;
            }

            UnMovableTransient & operator=(UnMovableTransient &&) = delete;

            bool operator==(UnMovableTransient const &oth) { return this == std::addressof(oth); }
        private:
            std::tuple<std::decay_t<As>...> _content;
    };
}

