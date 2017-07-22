#include <benchmark/benchmark.h>

#include "function-currying.hpp"

const auto abcdefg = [](auto a, auto b, auto c, auto d, auto e, auto f,
                        auto g) { return a + b + c + d + e + f + g; };

const auto devnull = [](auto... xs) -> decltype(auto) { return sizeof...(xs); };

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

static void bm_curry01X_calling(benchmark::State &state) {
  while (state.KeepRunning()) {
    curry01::curryX(devnull)("Call")("Me")("With")("any")("number")("of")(
        "args")(curry01::call);
  }
}
BENCHMARK(bm_curry01X_calling);

static void bm_curry02_calling(benchmark::State &state) {
  auto cur_callingbcdefg = curry02::curry(abcdefg);
  while (state.KeepRunning()) {
    cur_callingbcdefg(1)(2)(3)(4)(5)(6)(7);
  }
}
BENCHMARK(bm_curry02_calling);

BENCHMARK_MAIN();
