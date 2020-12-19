#pragma once
#include <memory>
#include <tuple>
#include <utility>

#include <utils/Counter.hpp>

namespace tests::types {
    template <class... As>
    class MovableSingleton: public utils::Counter<MovableSingleton<As...>> {
        public:
            using Counter = utils::Counter<MovableSingleton<As...>>;
            MovableSingleton(As... args): Counter(), _content(std::move(args)...) {}
            ~MovableSingleton() {};

            MovableSingleton(MovableSingleton const &) = delete;
            MovableSingleton(MovableSingleton && oth): Counter(std::forward<Counter>(oth)), _content(std::move(oth._content)) {}

            MovableSingleton & operator=(MovableSingleton const &) = delete;
            MovableSingleton & operator=(MovableSingleton && rhs) {
                _content = std::move(rhs._content);
                return *this;
            }

            bool operator==(MovableSingleton const &oth) { return this == std::addressof(oth); }

        private:
            std::tuple<std::decay_t<As>...> _content;
    };
}
