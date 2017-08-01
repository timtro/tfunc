#include "function-composition-and-piping.hpp"
#include <benchmark/benchmark.h>

#include <cmath>

// Things to test:
// * regular lambda
// * generic lambda
// * Function pointer
// * PMF

template <typename T>
struct Point2D {
  T x{0};
  T y{0};
};

template <typename T>
T l2norm2D(Point2D<T> p) {
  return std::sqrt(p.x * p.x + p.y * p.y);
}

// For PMF example
template <typename T>
struct getable {
  T N;
  getable(T N) : N(N) {}
  T get() const { return N; }
};

auto pythag34 = getable<Point2D<double>>({3., 4.});
auto g = [](auto x) { return x * 2; };
auto f = [](double x) { return x + 1; };

static void bm_by_hand_a(benchmark::State &state) {
  while (state.KeepRunning()) {
    f(g(l2norm2D(pythag34.get())));
  }
}
BENCHMARK(bm_by_hand_a);

static void bm_tf_a(benchmark::State &state) {
  auto fgh = compose01::compose(f, g, l2norm2D<double>,
                                &getable<Point2D<double>>::get);
  while (state.KeepRunning()) {
    fgh(&pythag34);
  }
}
BENCHMARK(bm_tf_a);

static void bm_compose01_a(benchmark::State &state) {
  auto fgh = compose01::compose(f, g, l2norm2D<double>,
                                &getable<Point2D<double>>::get);
  while (state.KeepRunning()) {
    fgh(&pythag34);
  }
}
BENCHMARK(bm_compose01_a);

static void bm_compose02_a(benchmark::State &state) {
  auto fgh = compose01::compose(f, g, l2norm2D<double>,
                                &getable<Point2D<double>>::get);
  while (state.KeepRunning()) {
    fgh(&pythag34);
  }
}
BENCHMARK(bm_compose02_a);

static void bm_pip01_a(benchmark::State &state) {
  while (state.KeepRunning()) {
    // clang-format off
    pipe01::pipe(&pythag34,
                    &getable<Point2D<double>>::get,
                    l2norm2D<double>,
                    g,
                    f);
    // clang-format on
  }
}
BENCHMARK(bm_pip01_a);

BENCHMARK_MAIN();
