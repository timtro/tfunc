#include <catch/catch.hpp>
#include <optional>

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

#include "../include/function-operations.hpp"
#include "../include/functor/flist.hpp"
#include "../include/functor/foptional.hpp"

using std::make_optional;
using std::nullopt;
using std::optional;

using tf::as_functor; // as_functor : G<A> → F<A>;
using tf::fmap;       // fmap : (A → B) → F<A> → F<B>

#include <algorithm>

TEST_CASE("The std::optional type constructor should be a functor:") {
  REQUIRE(tf::is_functor<std::vector>::value == true);
}

TEST_CASE("Given a std::optional<A> holding a value…", "[mathematical]") {

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

TEST_CASE("Given an empty std::optional<A>…", "[mathematical]") {
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

auto nccpy_to_cpy(const std::vector<CtorLogger::flat_t> &v) {
  std::vector<CtorLogger::flat_t> outv;
  outv.reserve(v.size());
  std::replace_copy_if(
      v.cbegin(), v.cend(), std::back_inserter(outv),
      [](auto x) { return x == CtorLogger::NCCopyConstructed; },
      CtorLogger::CopyConstructed);
  return outv;
}

TEST_CASE("The only difference between mapping id on an optional containing a "
          "value, and simply invoking id on a the dereferenced optional should "
          "be that fmap forces the use of a const-copy constructor.",
          "[CtorAs]") {
  optional<CtorLogger> oCtorLogger;
  oCtorLogger.emplace(); // flags = {Default}

  auto RawInvokeResult = make_optional(std::invoke(id, *oCtorLogger));

  oCtorLogger.emplace(); // refresh the logger to default
  REQUIRE(oCtorLogger->flags == std::vector{CtorLogger::Default});

  auto FmapResult = fmap(id, oCtorLogger);
  auto rawFlagsTransformed = nccpy_to_cpy(RawInvokeResult->flags);

  REQUIRE(FmapResult->flags != RawInvokeResult->flags);
  REQUIRE(FmapResult->flags == rawFlagsTransformed);
}
