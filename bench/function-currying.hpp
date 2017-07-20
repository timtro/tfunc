#include <experimental/type_traits>
#include <functional>

using std::experimental::is_detected_v;

// No include guards. If you're including these outside of a benchmark, then
// you're taking your life into your hands.

namespace curry01 {
  // Details: The simplest currying mechanism I could come up with. A simple
  // SFINAE trait using Walter Brown's detection idiom detects nullary
  // callables. The curry function, when called on a non-nullary callable
  // object, simply returns a variadic lambda object that will pack its given
  // argument(s) into a new variadic lambda object. This is packing and
  // returning of unary lambdas is recursive, until the last required argument
  // is passed. Then, the final lambda object is nullary, and simply invokes the
  // nested lambdas.

  namespace detail {
    template <typename T>
    using is_nullary_t = decltype(std::invoke(std::declval<T>()));

    template <typename T>
    constexpr bool is_nullary_v = is_detected_v<is_nullary_t, T>;
  } // namespace detail

  template <typename F>
  auto curry(F &&f) {
    if constexpr (detail::is_nullary_v<F>)
      return std::invoke(f);
    else
      return [=](auto &&x) {
        return curry([=](auto &&... xs) -> decltype(std::invoke(f, x, xs...)) {
          return std::invoke(f, x, xs...);
        });
      };
  }
} // namespace curry01
