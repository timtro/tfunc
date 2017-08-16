#include <catch/catch.hpp>

#include "test-fixtures.hpp"
using tst::A; // Tag for unit type
using tst::B; // Tag for unit type
using tst::C; // Tag for unit type
using tst::CtorLogger;
using tst::D;  // Tag for unit type
using tst::f;  // f : A → B
using tst::g;  // g : B → C
using tst::h;  // h : C → D
using tst::id; // id : T → T

constexpr auto Default = CtorLogger::Default;
constexpr auto CopyConstructed = CtorLogger::CopyConstructed;
constexpr auto MoveConstructed = CtorLogger::MoveConstructed;
constexpr auto MovedFrom = CtorLogger::MovedFrom;

#include "../include/function-operations.hpp"
#include "../include/functor/flist.hpp"
#include "../include/functor/foptional.hpp"

using std::experimental::make_optional;
using std::experimental::nullopt;
using std::experimental::optional;

using tf::as_functor; // as_functor : G<A> → F<A>;
using tf::fmap;       // fmap : (A → B) → F<A> → F<B>

#include "../include/functor/functor.hpp"

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

TEST_CASE("As it is currently implemented, a call to fmap should result in one "
          "move construction more than a simply calling invoke on a "
          "dereferenced optional value:") {
  optional<CtorLogger> ocl;
  ocl.emplace(); // ocl.flags = {Default}
  auto raw_invoke = std::invoke(
      id, *ocl); // flags = {Default, CopyConstructed, MoveConstructed}
  raw_invoke.flags.push_back(MoveConstructed);
  REQUIRE(fmap(id, ocl)->flags == raw_invoke.flags);
}
