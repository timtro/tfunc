#pragma once

#include "functor.hpp"

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

  // fmap : (A → B) → F<A> → F<B>
  //
  template <template <typename...> typename Functor, typename A,
            typename... FCtorArgs, typename F>
  auto fmap(F f, const Functor<A, FCtorArgs...> as) {
    Functor<invoke_result_t<F, A>> bs;

    if constexpr (dtl_::has_reserve_v<decltype(bs)>)
      bs.reserve(as.size());

    std::transform(cbegin(as), cend(as), std::back_inserter(bs),
                   std::forward<F>(f));
    return bs;
  }

  template <typename F, typename A, size_t N>
  auto fmap(F f, const std::array<A, N> &as) {
    std::array<invoke_result_t<F, A>, N> bs;

    std::transform(cbegin(as), cend(as), begin(bs), f);
    return bs;
  }

} // namespace tf
