#pragma once

namespace tests::utils{
    template <class T>
    class Counter {
        static int _CountCtor;
        static int _CountCCtor;
        static int _CountMCtor;

        public:
        Counter() {
            _CountCtor += 1;
        }

        Counter(Counter const &) {
            _CountCtor += 1;
            _CountCCtor += 1;
        }

        Counter(Counter &&) {
            _CountMCtor += 1;
        }

        static int getCount() { return _CountCtor; }
        static int getCopyCount() { return _CountCtor; }
        static int getMoveCount() { return _CountCtor; }
    };

    template <class T>
    int Counter<T>::_CountCtor = 0;
    template <class T>
    int Counter<T>::_CountCCtor = 0;
    template <class T>
    int Counter<T>::_CountMCtor = 0;
}
