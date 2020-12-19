#pragma once
#include <memory>
#include <tuple>
#include <utility>

#include <utils/Counter.hpp>

namespace tests::types {
    template <class... As>
    class UnCopiableTransient: public utils::Counter<UnCopiableTransient<As...>> {
        using Counter = utils::Counter<UnCopiableTransient<As...>>;
        public:
            UnCopiableTransient(As...) : Counter() {}
            virtual ~UnCopiableTransient();

            UnCopiableTransient(UnCopiableTransient const &) = delete;
            UnCopiableTransient(UnCopiableTransient &&oth): Counter(std::forward<Counter>(oth)), _content(std::move(oth._content)) {};

            UnCopiableTransient & operator=(UnCopiableTransient const &) = delete;
            UnCopiableTransient & operator=(UnCopiableTransient &&rhs) {
                if (std::addressof(*this) != std::addressof(rhs))
                    _content = std::move(rhs._content);
                return *this;
            }

            bool operator==(UnCopiableTransient const &oth) { return this == std::addressof(oth); }
        private:
            std::tuple<std::decay_t<As>...> _content;
    };
}
