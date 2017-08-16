#pragma once

#include <experimental/optional>

#include "functor.hpp"

#include "../function_traits.hpp"
#include <algorithm>
#include <experimental/type_traits>
#include <functional>
#include <type_traits>

using std::experimental::make_optional;
using std::experimental::optional;

// fmap : (A → B) → F<A> → F<B>
template <typename A, typename F>
auto fmap(F f, const optional<A> &oa) {
  if (oa) {
    return make_optional(std::invoke(f, *oa));
  } else
    return optional<trait::invoke_result_t<F, A>>{};
}

#include "curried-fmap.hpp"
