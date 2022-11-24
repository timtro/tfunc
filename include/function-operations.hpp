#pragma once

#include <experimental/type_traits>
#include <functional>
#include <type_traits>
#include <utility>

using std::experimental::is_detected_v;

namespace tf {

  template <typename T>
  constexpr decltype(auto) id(T &&x) {
    return std::forward<T>(x);
  }

  template <typename F, typename... Fs>
  constexpr decltype(auto) compose(F f, Fs... fs) {
    if constexpr (sizeof...(fs) < 1)
      return [f](auto &&x) -> decltype(auto) {
        return std::invoke(f, std::forward<decltype(x)>(x));
      };
    else
      return [f, fs...](auto &&x) -> decltype(auto) {
        return std::invoke(f, compose(fs...)(std::forward<decltype(x)>(x)));
      };
  }

  template <typename T, typename F0, typename... Fs>
  constexpr decltype(auto) pipe(T &&x, F0 f, Fs... gh) {
    if constexpr (sizeof...(Fs) < 1)
      return std::invoke(f, std::forward<decltype(x)>(x));
    else
      return pipe(std::invoke(f, std::forward<decltype(x)>(x)), gh...);
  }

  struct call_t {
  } call;

  template <typename F>
  constexpr decltype(auto) curry(F f) {
    if constexpr (std::is_invocable_v<F>)
      return std::invoke(f);
    else
      return [f](auto &&x) {
        return curry(
            // perfectly capture x here:
            [f, x](auto &&... xs) -> decltype(std::invoke(f, x, xs...)) {
              return std::invoke(f, x, xs...);
            });
      };
  }

  // Looks for a call_t as a sentinal to signal the end of the curry.
  template <typename F>
  constexpr decltype(auto) curry_variadic(F f) {
    return [f](auto x) -> decltype(auto) {
      if constexpr (std::is_same<std::decay_t<decltype(x)>, call_t>::value)
        return std::invoke(f);
      else
        return curry_variadic(
            // perfectly capture x here:
            [f, x](auto &&... xs) -> decltype(std::invoke(f, x, xs...)) {
              return std::invoke(f, x, xs...);
            });
    };
  }

} // namespace tf
