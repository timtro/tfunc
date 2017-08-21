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

  template <template <typename, std::size_t> class DFunctor, std::size_t N,
            typename A, typename B>
  using dfmap_t = decltype(
      fmap(std::declval<B (&)(A)>(), std::declval<DFunctor<A, N> const &>()));

  template <template <typename, std::size_t> class, std::size_t N,
            typename = void>
  struct is_dfunctor : std::false_type {};

  template <template <typename, std::size_t> class DFunctor, std::size_t N>
  struct is_dfunctor<DFunctor, N,
                     std::void_t<dfmap_t<DFunctor, N, DumA_, DumB_>>>
      : std::is_same<dfmap_t<DFunctor, N, DumA_, DumB_>, DFunctor<DumB_, N>> {};

} // namespace tf
