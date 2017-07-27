#include <algorithm>
#include <functional>
#include <iterator>

using std::begin;
using std::cbegin;
using std::cend;
using std::end;

namespace mlist01 {
  // This was my first attempt. I didn't know what I was doing, and I need to
  // refactor the whole shambles. But I wanted to get this file moving forward.

  namespace _dtl {
    template <typename Container>
    using value_type = typename Container::value_type;

    template <typename F, typename Container>
    using invoke_result = decltype(
        std::invoke(std::declval<F>(), std::declval<value_type<Container>>()));
  } // namespace _dtl

  template <typename Container, typename F>
  struct mutable_in_place
      : public std::bool_constant<
            std::is_same<_dtl::value_type<Container>,
                         _dtl::invoke_result<F, Container>>::value> {};

  template <typename T>
  constexpr auto has_reserve_memfun(T &container)
      -> decltype(container.reserve(1), bool{}) {
    return true;
  }
  constexpr bool has_reserve_memfun(...) { return false; }

  namespace _dtl {

    template <template <typename...> typename Container, typename T,
              typename... TArgs, typename F>
    auto fmap(F &&callable, Container<T, TArgs...> container, std::true_type) {
      std::transform(cbegin(container), cend(container), begin(container),
                     std::forward<F>(callable));

      return container;
    }

    template <template <typename...> typename Container, typename A,
              typename... AArgs, typename F>
    auto fmap(F &&callable, Container<A, AArgs...> &&container,
              std::false_type) {
      using parameter_type = decltype(*begin(container));

      using invoke_result = decltype(
          std::invoke(std::declval<F>(), std::declval<parameter_type>()));

      Container<invoke_result> mapped_container;
      if constexpr (has_reserve_memfun(mapped_container)) {
        mapped_container.reserve(container.size());
      }
      std::transform(cbegin(container), cend(container),
                     std::back_inserter(mapped_container),
                     std::forward<F>(callable));

      return mapped_container;
    }

    template <typename F, typename A, size_t N>
    auto fmap(F &&callable, std::array<A, N> xs, std::true_type) {
      std::transform(cbegin(xs), cend(xs), begin(xs),
                     std::forward<F>(callable));

      return xs;
    }

    template <typename F, typename A, size_t N>
    auto fmap(F &&callable, const std::array<A, N> &&xs, std::false_type) {
      using parameter_type = decltype(*begin(xs));
      using invoke_result = decltype(
          std::invoke(std::declval<F>(), std::declval<parameter_type>()));

      std::array<invoke_result, N> mapped_container;

      std::transform(cbegin(xs), cend(xs), begin(mapped_container),
                     std::forward<F>(callable));

      return mapped_container;
    }

  } // namespace _dtl

  template <typename Container, typename F>
  auto fmap(F &&callable, Container &&container) {
    return _dtl::fmap(std::forward<F>(callable),
                      std::forward<Container>(container),
                      mutable_in_place<std::decay_t<Container>, F>{});
  }

  // TODO: Rewrite this into an overload.
  template <typename F, typename A, size_t N>
  auto fmap(F &&callable, std::array<A, N> &&xs) {
    return _dtl::fmap(std::forward<F>(callable),
                      std::forward<std::array<A, N>>(xs),
                      mutable_in_place<std::array<A, N>, F>{});
  }

} // namespace mlist01
