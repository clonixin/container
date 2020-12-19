#pragma once
#include <memory>
#include <tuple>
#include <utility>

#include <utils/Counter.hpp>

namespace tests::types {
    template <class... As>
    class CopiableSingleton: public utils::Counter<CopiableSingleton<As...>> {
        public:
            using Counter = utils::Counter<CopiableSingleton<As...>>;
            CopiableSingleton(As... args): Counter(), _content(std::move(args)...) {}
            ~CopiableSingleton() {};

            CopiableSingleton(CopiableSingleton const &oth): Counter(std::forward<Counter const>(oth)), _content(oth._content) {}
            CopiableSingleton(CopiableSingleton &&) = delete;

            CopiableSingleton & operator=(CopiableSingleton const &rhs) {
                _content = rhs._content;
                return *this;
            };
            CopiableSingleton & operator=(CopiableSingleton &&) = delete;

            bool operator==(CopiableSingleton const &oth) { return this == std::addressof(oth); }

        private:
            std::tuple<std::decay_t<As>...> _content;
    };
}
