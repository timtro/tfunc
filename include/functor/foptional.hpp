#pragma once

#include "../function_traits.hpp"
#include "functor.hpp"
#include <algorithm>
#include <optional>
#include <type_traits>
#include <functional>
#include <type_traits>

using std::make_optional;
using std::optional;

// fmap : (A → B) → F<A> → F<B>
//
// Accepts optional value by reference, and then dereferences on pass to invoke.
template <typename A, typename F>
auto fmap(F f, const optional<A> &oa) {
  if (oa) {
    return make_optional(std::invoke(f, *oa));
  } else
    // TODO: Replace with std::invoke_result_t when Clang implements it.
    return optional<trait::invoke_result_t<F, A>>{};
}

// fmap : (A → B) → F<A> → F<B>
//
// Takes optional by universal reference and moves from the dereferenced
// optional.
template <typename A, typename F>
auto fmap(F f, optional<A> &&oa) {
  if (oa) {
    return make_optional(std::invoke(f, std::move(*oa)));
  } else
    // TODO: Replace with std::invoke_result_t when Clang implements it.
    return optional<trait::invoke_result_t<F, A>>{};
}

#include "curried-fmap.hpp"
