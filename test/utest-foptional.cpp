#include <catch/catch.hpp>

#include "../include/function-operations.hpp"
#include "../include/functor/flist.hpp"
#include "../include/functor/foptional.hpp"
#include "test-fixtures.hpp"

using tf::as_functor;
using tf::fmap;

// f : A → B
const auto f = [](A) -> B { return {}; };
// g : B → C
const auto g = [](B) -> C { return {}; };
// h : C → D
const auto h = [](C) -> D { return {}; };
// id : A → A
const auto id = [](auto x) { return x; };

TEST_CASE("The std::optional type constructor should be a functor:") {
  REQUIRE(tf::is_functor<std::vector>::value == true);
}
