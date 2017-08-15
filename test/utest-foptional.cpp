#include <catch/catch.hpp>

#include "../include/function-operations.hpp"
#include "../include/functor/flist.hpp"
#include "../include/functor/foptional.hpp"
#include "test-fixtures.hpp"

using tf::as_functor;
using tf::fmap;

using std::experimental::make_optional;
using std::experimental::nullopt;
using std::experimental::optional;

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

TEST_CASE("Given a std::optional<A> holding a value…") {

  optional<A> oa{A{}};

  SECTION("… and a function f : A → B, fmap should produce an occupied "
          "std::optional<B>:") {
    REQUIRE(fmap(f, oa) == make_optional(B{}));
    REQUIRE(*fmap(f, oa) == B{});
  }

  SECTION("… it should remain unchaged when fmaped with id.") {
    REQUIRE(fmap(id, oa) == oa);
    REQUIRE(*fmap(id, oa) == *oa);
  }

  SECTION("… fmap should obey the functor composition law:") {
    REQUIRE(fmap(g, fmap(f, oa)) == make_optional(C{}));
    REQUIRE(fmap(g, fmap(f, oa)) == fmap(tf::compose(g, f), oa));
  }
}

TEST_CASE("Given an empty std::optional<A>…") {
  optional<A> oa;

  SECTION("… and a function f : A → B, fmap should produce an empty "
          "std::optional<B>:") {
    REQUIRE(fmap(f, oa) == optional<B>{});
  }

  SECTION("… it should remain unchaged when fmaped with id.") {
    REQUIRE(fmap(id, oa) == nullopt);
    REQUIRE(fmap(id, oa) == oa);
  }

  SECTION("… fmap should obey the functor composition law:") {
    REQUIRE(fmap(g, fmap(f, oa)) == optional<C>{});
    REQUIRE(fmap(g, fmap(f, oa)) == fmap(tf::compose(g, f), oa));
  }
}
