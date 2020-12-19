#pragma once
#include <memory>
#include <tuple>
#include <utility>

#include <utils/Counter.hpp>

namespace tests::types {
    template <class... As>
    class Transient: public utils::Counter<Transient<As...>> {
        using Counter = utils::Counter<Transient<As...>>;
        public:
            Transient(As... args) : Counter(), _content(std::move(args)...) {}
            virtual ~Transient() {}

            Transient(Transient const &oth): Counter(std::forward<Counter const>(oth)), _content(oth._content) {}
            Transient(Transient &&oth): Counter(std::forward<Counter>(oth)), _content(std::move(oth._content)) {}

            Transient & operator=(Transient const & rhs) {
                if (std::addressof(*this) != std::addressof(rhs))
                    _content = rhs._content;
                return *this;
            }

            Transient & operator=(Transient &&rhs) {
                if (std::addressof(*this) != std::addressof(rhs))
                    _content = std::move(rhs._content);
                return *this;
            }

            bool operator==(Transient const &oth) { return this == std::addressof(oth); }

        private:
            std::tuple<std::decay_t<As>...> _content;
    };
}
