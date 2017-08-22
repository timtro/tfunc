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

  // Will is_functor be true_type if an fmap overload exists with the correct
  // signature ( fmap( B(A), F<A>) and it produces an F<B>.
  template <template <typename, typename...> class Functor>
  struct is_functor<Functor, std::void_t<fmap_t<Functor, DumA_, DumB_>>>
      : std::is_same<fmap_t<Functor, DumA_, DumB_>, Functor<DumB_>> {};

  // The std::array constructor, unlike the other std containers, needs a
  // size_t (natural number : N) specifying the size of the array. This makes it
  // a dependent type of the product (pi) variety. The template signature for
  // the type constrcutor conflics with those for other std containers so this
  // set of meta/functions works for types that need a size_t to construct them.
  //
  template <template <typename, std::size_t> class Functor, std::size_t N,
            typename A, typename B>
  using fmap_N_t = decltype(
      fmap(std::declval<B (&)(A)>(), std::declval<Functor<A, N> const &>()));

  template <template <typename, std::size_t> class, typename = void>
  struct is_functor_N : std::false_type {};

  template <template <typename, std::size_t> class Functor>
  struct is_functor_N<
      Functor, std::void_t<fmap_N_t<Functor, std::size_t{}, DumA_, DumB_>>>
      : std::is_same<fmap_N_t<Functor, std::size_t{}, DumA_, DumB_>,
                     Functor<DumB_, std::size_t{}>> {};

} // namespace tf
