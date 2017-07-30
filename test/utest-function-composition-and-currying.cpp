#include "../include/function-composition-and-currying.hpp"

#include <catch/catch.hpp>
#include <string>

using tf::compose;

template <typename T>
T id(T &&xs) {
  return std::forward<T>(xs);
}

TEST_CASE("`compose(f)` == `compose(f, id)` == `compose(id, f)`.") {
  auto f = [](auto x) { return x * x; };
  REQUIRE(compose(f)(-5) == compose(f, id<int>)(-5));
  REQUIRE(compose(f, id<int>)(-5) == compose(id<int>, f)(-5));
}

TEST_CASE("Two composed integer-identity lambdas should identify.") {
  auto id2 = compose([](int x) { return x; }, [](int x) { return x; });
  REQUIRE(id2(-1) == -1);
  REQUIRE(id2(0) == 0);
  REQUIRE(id2(1) == 1);
  REQUIRE(id2(10) == 10);
}

TEST_CASE("Two composed identity C-functions should identify") {
  auto id2 = compose(id<int>, id<int>);
  REQUIRE(id2(-1) == -1);
  REQUIRE(id2(0) == 0);
  REQUIRE(id2(1) == 1);
  REQUIRE(id2(10) == 10);
}
