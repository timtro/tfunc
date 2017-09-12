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

  // A static interface for any functor. The idea here is to provide an fmap
  // member for standard containers, like std::vector and std::options. Use:
  //  std::vector<int> a{…};
  //  as_functor{a}.fmap(…);
  // but maybe this is a bad idea.
  template <typename Functor>
  struct Functor_t {
    const Functor &as;

    Functor_t(const Functor &as) : as{as} {}

    template <typename F>
    decltype(auto) fmap(F &&f) const {
      return tf::fmap(std::forward<F>(f), as);
    }
  };

  template <typename Functor>
  auto as_functor(const Functor &as) {
    return Functor_t<Functor>{as};
  }
} // namespace tf
