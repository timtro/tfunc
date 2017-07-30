#include <experimental/type_traits>
#include <functional>
#include <type_traits>
#include <utility>

using std::experimental::is_detected_v;

namespace tf {

  template <typename F, typename... Fs>
  auto compose(F f, Fs... fs) {
    if constexpr (sizeof...(fs) < 1)
      return [f](auto &&x) {
        return std::invoke(f, std::forward<decltype(x)>(x));
      };
    else
      return [f, fs...](auto &&x) {
        return std::invoke(f, compose(fs...)(std::forward<decltype(x)>(x)));
      };
  }

  template <typename T, typename F0, typename... Fs>
  auto pipe(T &&x, F0 f, Fs... gh) {
    if constexpr (sizeof...(Fs) < 1)
      return std::invoke(f, std::forward<decltype(x)>(x));
    else
      return pipe(std::invoke(f, std::forward<decltype(x)>(x)), gh...);
  }

  struct call_t {
  } call;

  namespace _dtl {
    template <typename T>
    using is_nullary_t = decltype(std::invoke(std::declval<T>()));

    template <typename T>
    constexpr bool is_nullary_v = is_detected_v<is_nullary_t, T>;
  } // namespace _dtl

  template <typename F>
  auto curry(F f) {
    if constexpr (_dtl::is_nullary_v<F>)
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

  template <typename F>
  auto curryX(F f) {
    return [f](auto x) {
      if constexpr (std::is_same<std::decay_t<decltype(x)>, call_t>::value)
        return std::invoke(f);
      else
        return curryX(
            // perfectly capture x here:
            [f, x](auto &&... xs) -> decltype(std::invoke(f, x, xs...)) {
              return std::invoke(f, x, xs...);
            });
    };
  }

} // namespace tf
