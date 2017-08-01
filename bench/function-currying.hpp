#include <experimental/type_traits>
#include <functional>
#include <type_traits>
#include <utility>

using std::experimental::is_detected_v;

// clang-format on

// No include guards. If you're including these outside of a benchmark, then
// you're taking your life into your hands.

namespace curry01 {
  // Details: The simplest currying mechanism I could come up with. A simple
  // SFINAE trait using Walter Brown's detection idiom detects nullary
  // callables. The curry function, when called on a non-nullary callable
  // object, simply returns a variadic lambda object that will pack its given
  // argument(s) into a new variadic lambda object. This is packing and
  // returning of lambdas is recursive, until the last required argument
  // is passed. Then, the final lambda object is nullary, and simply invokes the
  // nested lambdas.

  struct call_t {
  } call;

  namespace dtl_ {
    template <typename T>
    using is_nullary_t = decltype(std::invoke(std::declval<T>()));

    template <typename T>
    constexpr bool is_nullary_v = is_detected_v<is_nullary_t, T>;
  } // namespace dtl_

  template <typename F>
  auto curry(F f) {
    if constexpr (dtl_::is_nullary_v<F>)
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

  // This allows you to curry variadic functions. I got the idea of this from
  // https://goo.gl/gBvBmx. This returns a varidac callable which binds
  // arguments, until an argument of type call_t is passed. Passing such forces
  // the an invocation of the curried function with all previously bound
  // arguments.
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
} // namespace curry01

namespace curry02 {
  // The only difference between curry02 and curry01 is the use of std::invoke.
  // Any difference in performance should be due to the universal calling
  // overhead.
  //
  namespace dtl_ {
    template <typename T>
    using is_nullary_t = decltype(std::invoke(std::declval<T>()));

    template <typename T>
    constexpr bool is_nullary_v = is_detected_v<is_nullary_t, T>;
  } // namespace dtl_

  template <typename F>
  auto curry(F f) {
    if constexpr (dtl_::is_nullary_v<F>)
      return f();
    else
      return [f](auto &&x) {
        return curry(
            // perfectly capture x here:
            [f, x](auto &&... xs) -> decltype(f(x, xs...)) {
              return f(x, xs...);
            });
      };
  }
} // namespace curry02
