#pragma once
#include <memory>
#include <tuple>
#include <utility>

#include <utils/Counter.hpp>

namespace tests::types {
    template <class... As>
    class BadSingleton: public utils::Counter<BadSingleton<As...>> {
        using Counter = utils::Counter<BadSingleton<As...>>;
        public:
            BadSingleton(As... args) : Counter(), _content(std::move(args)...) {}
            virtual ~BadSingleton() {}

            BadSingleton(BadSingleton const &oth): Counter(std::forward<Counter const>(oth)), _content(oth._content) {}
            BadSingleton(BadSingleton &&oth): Counter(std::forward<Counter>(oth)), _content(std::move(oth._content)) {}

            BadSingleton & operator=(BadSingleton const & rhs) {
                if (std::addressof(*this) != std::addressof(rhs))
                    _content = rhs._content;
                return *this;
            }

            BadSingleton & operator=(BadSingleton &&rhs) {
                if (std::addressof(*this) != std::addressof(rhs))
                    _content = std::move(rhs._content);
                return *this;
            }

            bool operator==(BadSingleton const &oth) { return this == std::addressof(oth); }

        private:
            std::tuple<std::decay_t<As>...> _content;
    };
}
