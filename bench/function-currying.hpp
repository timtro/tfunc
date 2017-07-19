#include <functional>

// No include guards. If you're including these outside of a benchmark, then
// you're taking your life into your hands.

namespace curry01 {
  // Details: The simplest currying mechanism I could come up with. A SFINAE
  // based trait detects a callable's need for arguments. The curry function,
  // when called on a callable object, simply returns a variadic lambda object
  // that will pack its argument(s) into a new variadic lambda object. This is
  // packing and returning of unary lambdas is recursive, until the last
  // required argument is passed. Then, the final lambda object is nullary, and
  // simply invokes the nested lambdas.

  namespace detail {
    template <class, class = void>
    struct needs_arguments : std::true_type {};

    template <class T>
    struct needs_arguments<
        T, std::void_t<decltype(std::invoke(std::declval<T>()))>>
        : std::false_type {};

    template <typename T>
    constexpr bool needs_arguments_v = needs_arguments<T>::value;
  } // namespace detail

  template <typename F>
  auto curry(F &&f) {
    if constexpr (detail::needs_arguments_v<decltype(f)>)
      return [=](auto &&x) {
        return curry([=](auto &&... xs) -> decltype(std::invoke(f, x, xs...)) {
          return std::invoke(f, x, xs...);
        });
      };
    else
      return std::invoke(f);
  }

} // namespace curry01
