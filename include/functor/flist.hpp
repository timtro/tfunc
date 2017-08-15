#pragma once

#include "../function_traits.hpp"
#include "functor.hpp"

#include <algorithm>
#include <experimental/type_traits>
#include <iterator>
#include <type_traits>

#include "../function_traits.hpp"

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

  // fmap : (A → B) → F<A> → F<B>
  //
  template <template <typename...> typename Functor, typename A,
            typename... FCtorArgs, typename F>
  auto fmap(F f, const Functor<A, FCtorArgs...> &as) {
    Functor<trait::invoke_result_t<F, A>> bs;

    if constexpr (dtl_::has_reserve_v<decltype(bs)>)
      bs.reserve(as.size());

    std::transform(cbegin(as), cend(as), std::back_inserter(bs),
                   std::forward<F>(f));
    return bs;
  }

  template <typename F, typename A, size_t N>
  auto fmap(F f, const std::array<A, N> &as) {
    std::array<trait::invoke_result_t<F, A>, N> bs;

    std::transform(cbegin(as), cend(as), begin(bs), f);
    return bs;
  }

  // A static interface for any functor.
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
