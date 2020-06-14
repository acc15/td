#pragma once

#include <functional>

namespace td {

template<typename L, typename F>
struct fn_cast {
    static auto to_fn([[maybe_unused]] L* l, F f) {
        return std::function(f);
    }
};

template<typename L, typename S>
struct fn_cast<L, std::function<S>> {
    static std::function<S> to_fn([[maybe_unused]] L* l, const std::function<S>& f) {
        return f;
    }
};

template<typename L, typename D, typename Ret, typename ... Args>
struct fn_cast<L, Ret (D::*)(Args...)> {
    static std::function<Ret(Args...)> to_fn(L* l, Ret(D::*h)(Args...)) {
        return [l, h](Args... args) -> Ret { return (dynamic_cast<D*>(l)->*h)(args...); };
    }
};

}