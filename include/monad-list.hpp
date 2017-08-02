#include <algorithm>
#include <experimental/type_traits>
#include <functional>
#include <iterator>
#include <type_traits>

using std::begin;
using std::cbegin;
using std::cend;
using std::end;

using std::experimental::is_detected_v;

namespace tf {

  namespace dtl_ {
    template <class T>
    using has_reserve_t =
        decltype(std::declval<T &>().reserve(std::declval<size_t>()));

    template <typename T>
    constexpr bool has_reserve_v = is_detected_v<has_reserve_t, T>;
  } // namespace dtl_

  template <template <typename...> typename Container, typename T,
            typename... TArgs, typename F>
  auto fmap(F &&f, const Container<T, TArgs...> xs) {

    using parameter_type = decltype(*begin(xs));
    using invoke_result = decltype(
        std::invoke(std::declval<F>(), std::declval<parameter_type>()));

    Container<invoke_result> ys;

    if constexpr (dtl_::has_reserve_v<decltype(ys)>)
      ys.reserve(xs.size());

    std::transform(cbegin(xs), cend(xs), std::back_inserter(ys),
                   std::forward<F>(f));
    return ys;
  }

  template <typename F, typename A, size_t N>
  auto fmap(F &&f, const std::array<A, N> &xs) {
    using parameter_type = decltype(*begin(xs));
    using invoke_result = decltype(
        std::invoke(std::declval<F>(), std::declval<parameter_type>()));

    std::array<invoke_result, N> ys;

    std::transform(cbegin(xs), cend(xs), begin(ys), std::forward<F>(f));

    return ys;
  }

  // A delayed-binding wrapper producing a curried fmap variant.
  template <typename F>
  auto fmap(F f) {
    return [f](auto xs) -> decltype(auto) { return fmap(f, xs); };
  }

} // namespace tf
