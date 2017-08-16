#pragma once

#include <type_traits>
#include <utility>

// Dummy unit types for traits and tests:
struct DumA_ {};
struct DumB_ {};

namespace tf {
  // fmap_t<> : a metafunction to deduce F<B> in:
  //   fmap: (A → B) → F<A> → F<B>
  template <template <typename, typename...> class Functor, typename A,
            typename B>
  using fmap_t = decltype(
      fmap(std::declval<B (&)(A)>(), std::declval<Functor<A> const &>()));

  // is_functor<> : a metafunction to test if a given type constructor is a
  // Functor. A std::true_type is constructed if, for a candidate type
  // constructor F, the result of calling fmap(A→B, F<A>) is an F<B>.
  template <template <typename, typename...> class, typename = void>
  struct is_functor : std::false_type {};

  template <template <typename, typename...> class Functor>
  struct is_functor<Functor, std::void_t<fmap_t<Functor, DumA_, DumB_>>>
      : std::is_same<fmap_t<Functor, DumA_, DumB_>, Functor<DumB_>> {};

} // namespace tf
