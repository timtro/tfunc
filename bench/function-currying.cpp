#include <benchmark/benchmark.h>

#include "function-currying.hpp"

auto abcdefg = [](auto a, auto b, auto c, auto d, auto e, auto f, auto g) {
  return a + b + c + d + e + f + g;
};

static void bm_byhand_calling(benchmark::State &state) {
  while (state.KeepRunning()) {
    abcdefg(1, 2, 3, 4, 5, 6, 7);
  }
}
BENCHMARK(bm_byhand_calling);

static void bm_curry01_calling(benchmark::State &state) {
  auto cur_callingbcdefg = curry01::curry(abcdefg);
  while (state.KeepRunning()) {
    cur_callingbcdefg(1)(2)(3)(4)(5)(6)(7);
  }
}
BENCHMARK(bm_curry01_calling);

static void bm_curry01_creation(benchmark::State &state) {
  while (state.KeepRunning()) {
    auto cur_callingbcdefg = curry01::curry(abcdefg);
    cur_callingbcdefg(1)(2)(3)(4)(5)(6)(7);
  }
}
BENCHMARK(bm_curry01_creation);

BENCHMARK_MAIN();
