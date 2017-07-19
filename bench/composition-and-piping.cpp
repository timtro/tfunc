#include "composition-and-piping.hpp"
#include <benchmark/benchmark.h>

template <typename T>
T square(T x) {
  return x * x;
}

auto f = [](auto x) { return x + 1; };
auto g = [](int x) { return x * 1; };

static void bm_compose01_a(benchmark::State &state) {
  auto fgh = compose01::compose(f, g, square<int>);
  while (state.KeepRunning()) {
    fgh(4);
  }
}
BENCHMARK(bm_compose01_a);

static void bm_compose02_a(benchmark::State &state) {
  auto fgh = compose02::compose(f, g, square<int>);
  while (state.KeepRunning()) {
    fgh(4);
  }
}
BENCHMARK(bm_compose02_a);

BENCHMARK_MAIN();
