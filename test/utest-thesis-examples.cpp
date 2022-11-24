#include <catch2/catch.hpp>

#include <cmath>
#include <optional>
#include <type_traits>
#include <vector>

#include "test-fixtures.hpp"
using tst::A;  // Tag for unit type
using tst::B;  // Tag for unit type
using tst::C;  // Tag for unit type
using tst::CtorLogger;
using tst::D;  // Tag for unit type
using tst::f;  // f : A → B
using tst::g;  // g : B → C
using tst::h;  // h : C → D

#include "../include/function-operations.hpp"

using tf::compose;
using tf::curry;
using tf::curry_variadic;
using tf::id;
using tf::pipe;

#include "../include/functor/flist.hpp"
#include "../include/functor/foptional.hpp"

#include "../include/functor/curried-fmap.hpp"

using tf::as_functor;  // as_functor : G<A> → F<A>;
using tf::fmap;        // fmap : (A → B) → F<A> → F<B>

TEST_CASE("Example from list-functor") {
  const std::vector<double> nums = {0.1, 0.2, 0.3, 0.4, 0.5};
  constexpr auto sin = [](double x) { return std::sin(x); };
  // alternatively:
  //   constexpr auto sin = static_cast<double (*)(double)>(std::sin);
  constexpr auto sqr = [](double x) { return x * x; };

  SECTION("Sine of nums") {
    const auto vec_sin = fmap(sin);
    auto sinNums = vec_sin(nums);
    REQUIRE(sinNums.at(0) == std::sin(nums.at(0)));
    REQUIRE(sinNums.at(1) == std::sin(nums.at(1)));
    REQUIRE(sinNums.at(2) == std::sin(nums.at(2)));
    REQUIRE(sinNums.at(3) == std::sin(nums.at(3)));
    REQUIRE(sinNums.at(4) == std::sin(nums.at(4)));
  }

  SECTION("Sine-square of nums") {
    // auto vec_sin_sqr = fmap(compose(sqr, sin));
    // auto sinSqrNums = vec_sin_sqr(nums);

    auto sinSqrNums = fmap(sqr)(fmap(sin)(nums));

    REQUIRE(sinSqrNums.at(0) == sqr(std::sin(nums.at(0))));
    REQUIRE(sinSqrNums.at(1) == sqr(std::sin(nums.at(1))));
    REQUIRE(sinSqrNums.at(2) == sqr(std::sin(nums.at(2))));
    REQUIRE(sinSqrNums.at(3) == sqr(std::sin(nums.at(3))));
    REQUIRE(sinSqrNums.at(4) == sqr(std::sin(nums.at(4))));
  }
}

TEST_CASE("Example from Maybe-functor") {
  auto safe_divide = [](int a, int b) -> std::optional<int> {
    if (b == 0)
      return {};
    else
      return {a / b};
  };
  auto sqr = [](int x) -> int { return x * x; };

  SECTION("Fmap over integer value"){
    auto result = fmap(sqr, safe_divide(18, 7));
    REQUIRE(*result == (18 / 7) * (18 / 7));
  }

  SECTION("Fmap over nullopt"){
    auto result = fmap(sqr, safe_divide(3, 0));
    REQUIRE(result == std::nullopt);
  }
}
