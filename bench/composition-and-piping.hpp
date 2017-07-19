#include <functional>
#include <tuple>

namespace compose01 {
  // Details: The simplest, most straightforward implementation I could think
  // of. It uses simple recursion to nest the functions into eachother like
  // матрёшка. The calls are wrapped in lambdas and the outer lambda handles
  // the input argument(s).
  //
  // template <typename F>
  // auto compose(F f) {
  //   return [f](auto x) { return f(x); };
  // }

  template <typename F, typename... Fs>
  auto compose(F f, Fs... fs) {
    if constexpr (sizeof...(fs) < 1)
      return [f](auto x) { return std::invoke(f, x); };
    else
      return [=](auto x) { return std::invoke(f, compose(fs...)(x)); };
  }

} // namespace compose01

namespace compose02 {
  // Details: implementation hinges on a function object containing a tuple of
  // the composed functions.
  //
  // I borrowed bits and pieces from several places. I got the idea to use a
  // tuple from Milewski and Neibler and their talk from C++Now! 2012:
  //   https://www.youtube.com/watch?v=WtHWFoKf8o0
  // Most of the implementation was borrowed from
  //   https://codereview.stackexchange.com/a/63893/83811

  template <typename... Fs>
  struct composition_fobject {

    std::tuple<Fs...> functions;

    composition_fobject(Fs &&... fs) : functions(std::forward<Fs>(fs)...) {}

    template <size_t N, typename... Ts>
    auto _invoke_each(std::integral_constant<size_t, N>, Ts &&... ts) const {
      if constexpr (N > 0) {
        static_assert(N <= std::tuple_size<decltype(functions)>::value);
        return _invoke_each(
            std::integral_constant<size_t, N - 1>{},
            std::invoke(std::get<N>(functions), std::forward<Ts>(ts)...));
      } else {
        static_assert(N >= 0);
        return std::invoke(std::get<0>(functions), std::forward<Ts>(ts)...);
      }
    }

    template <typename... Ts>
    auto operator()(Ts &&... ts) const {
      return _invoke_each(std::integral_constant<size_t, sizeof...(Fs) - 1>{},
                          std::forward<Ts>(ts)...);
    }
  };

  template <typename... Fs>
  auto compose(Fs... fs) {
    return composition_fobject<std::decay_t<Fs>...>(std::forward<Fs>(fs)...);
  }

  // TODO: confirm copy. I'm foggy on the mecahnics of the decay_t thing.
  // NB: from N2609: "Simply put, decay<T>::type is the identity
  // type-transformation except if T is an array type or a reference to a
  // function type. In those cases the decay<T>::type yields a pointer or a
  // pointer to a function, respectively."
  //
  // I'm going to integrate perfect-forwarding/capture into this as soon as I
  // find a good implementation.

} // namespace compose02

namespace compose03 {
  // Details: define composition as a binary operator and use
  template <typename Tf, typename Tg>
  auto fog(Tf &&f, Tg &&g) {
    return [f, g](auto x) {
      return std::invoke(f, std::invoke(g, std::forward<decltype(x)>(x)));
    };
  }
} // namespace compose03

namespace pipe01 {
  // Details: In the same vein as compose01, this is the simplest pipe I could
  // come up with.
  template <typename T, typename F0, typename... Fs>
  auto pipe(T x, F0 f, Fs... gh) {
    puts(__PRETTY_FUNCTION__);
    if constexpr (sizeof...(Fs) < 1)
      return std::invoke(f, x);
    else
      return pipe(std::invoke(f, x), gh...);
  }
} // namespace pipe01
