#pragma once
#include <memory>
#include <tuple>
#include <utility>

#include <utils/Counter.hpp>

namespace tests::types {
    template <class... As>
    class Singleton: public utils::Counter<Singleton<As...>> {
        public:
            using Counter = utils::Counter<Singleton<As...>>;
            Singleton(As... args): Counter(), _content(std::move(args)...) {}
            ~Singleton() {};

            Singleton(Singleton const &) = delete;
            Singleton(Singleton &&) = delete;

            Singleton & operator=(Singleton const &) = delete;
            Singleton & operator=(Singleton &&) = delete;

            bool operator==(Singleton const &oth) { return this == std::addressof(oth); }
        private:
            std::tuple<std::decay_t<As>...> _content;
    };
}
