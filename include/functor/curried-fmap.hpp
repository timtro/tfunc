#pragma once

#include <utility>

namespace tf {
  // A delayed-binding wrapper producing a curried fmap variant. That is to say,
  // this curried version of fmap produces a function lifted into a functor, but
  // the choice of which functor is delayed until the lifted function object is
  // passed a value from a specific functor and the fmap overload is selected.
  template <typename F>
  auto fmap(F f) {
    return [f](auto &&xs) -> decltype(auto) {
      return fmap(f, std::forward<decltype(xs)>(xs));
    };
  }
} // namespace tf
