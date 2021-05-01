#pragma once

namespace blip {

template <typename F, typename S>
struct Pair {
    F f;
    S s;

    Pair() {};
    Pair(F _f, S _s) : f(_f), s(_s) {};

    bool operator < (Pair<F, S> p) {
        return f < p.f || (f == p.f && s < p.s);
    }
};

template <typename F, typename S>
void swap(Pair<F, S> pair) {
        F h = pair.f;
        pair.f = pair.s;
        pair.s = h;
}

}
