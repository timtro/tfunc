#pragma once

#include <experimental/type_traits>
#include <functional>
#include <type_traits>

namespace trait {

  namespace dtl_ {
    template <typename AlwaysVoid, typename, typename...>
    struct invoke_result {};
    template <typename F, typename... Args>
    struct invoke_result<decltype(void(std::invoke(std::declval<F>(),
                                                   std::declval<Args>()...))),
                         F, Args...> {
      using type =
          decltype(std::invoke(std::declval<F>(), std::declval<Args>()...));
    };
  }  // namespace dtl_

  template <class F, class... ArgTypes>
  struct invoke_result : dtl_::invoke_result<void, F, ArgTypes...> {};

  template <class F, class... ArgTypes>
  using invoke_result_t = typename invoke_result<F, ArgTypes...>::type;

}  // namespace trait
