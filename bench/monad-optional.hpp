#include <experimental/optional>
#include <iostream>
#include <memory>
#include <string>
#include <type_traits>

#include <iostream>

namespace moptional01 {

  using std::experimental::make_optional;
  using std::experimental::optional;

  template <typename F, typename... Xs>
  using invoke_result_t =
      decltype(std::invoke(std::declval<F>(), std::declval<Xs...>()));

  template <typename F, typename A>
  auto fmap(F f, optional<A> p) -> optional<invoke_result_t<F, A>> {
    if (p)
      return make_optional(std::invoke(f, *p));
    else
      return {};
  }
} // namespace moptional01
