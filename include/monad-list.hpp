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

    template <typename AlwaysVoid, typename, typename...>
    struct invoke_result {};
    template <typename F, typename... Args>
    struct invoke_result<decltype(void(std::invoke(std::declval<F>(),
                                                   std::declval<Args>()...))),
                         F, Args...> {
      using type =
          decltype(std::invoke(std::declval<F>(), std::declval<Args>()...));
    };
  } // namespace dtl_

  template <class F, class... ArgTypes>
  struct invoke_result : dtl_::invoke_result<void, F, ArgTypes...> {};

  template <class F, class... ArgTypes>
  using invoke_result_t = typename invoke_result<F, ArgTypes...>::type;

  template <template <typename...> typename Container, typename T,
            typename... TArgs, typename F>
  auto fmap(F f, const Container<T, TArgs...> xs) {

    using parameter_type = decltype(*begin(xs));
    Container<invoke_result_t<F, parameter_type>> ys;

    if constexpr (dtl_::has_reserve_v<decltype(ys)>)
      ys.reserve(xs.size());

    std::transform(cbegin(xs), cend(xs), std::back_inserter(ys),
                   std::forward<F>(f));
    return ys;
  }

  template <typename F, typename A, size_t N>
  auto fmap(F f, const std::array<A, N> &xs) {
    using parameter_type = decltype(*begin(xs));
    std::array<invoke_result_t<F, parameter_type>, N> ys;

    std::transform(cbegin(xs), cend(xs), begin(ys), f);
    return ys;
  }

  // A delayed-binding wrapper producing a curried fmap variant.
  template <typename F>
  auto fmap(F f) {
    return [f](auto &&xs) -> decltype(auto) {
      return fmap(f, std::forward<decltype(xs)>(xs));
    };
  }

} // namespace tf
