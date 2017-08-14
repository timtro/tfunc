#include <catch/catch.hpp>

#include "../include/functor/flist.hpp"
#include "test-fixtures.hpp"

#include <type_traits>

#include <array>
#include <deque>
#include <list>
#include <valarray>
#include <vector>

#include <map>

using tf::fmap;

// f : A → B
const auto f = [](A) -> B { return {}; };
// g : B → C
const auto g = [](B) -> C { return {}; };
// h : C → D
const auto h = [](C) -> D { return {}; };

TEST_CASE("The std::vector<> typeconstructor should be a functor") {
  REQUIRE(tf::is_functor<std::vector>::value == true);
}

TEST_CASE("The std::list<> typeconstructor should be a functor") {
  REQUIRE(tf::is_functor<std::list>::value == true);
}

TEST_CASE("The std::array<> typeconstructor should be a functor") {
  REQUIRE(tf::is_functor<std::list>::value == true);
}

TEST_CASE("The std::deque<> typeconstructor should be a functor") {
  REQUIRE(tf::is_functor<std::list>::value == true);
}

// Dynamic tests that is_functor already tests statically.
TEST_CASE("Given a standard library sequence container of As, and a function f "
          ": A → B, fmap should produce a container of Bs, for …") {
  SECTION("… std::vector") {
    std::vector as{A{}};
    auto bs = fmap(f, as);
    REQUIRE((std::is_same<decltype(bs)::value_type, B>::value));
    REQUIRE(bs.at(0) == B{});
  }
  SECTION("… std::list") {
    std::list as{A{}};
    auto bs = fmap(f, as);
    REQUIRE((std::is_same<decltype(bs)::value_type, B>::value));
    REQUIRE(*bs.cbegin() == B{});
  }
  SECTION("… std::array") {
    std::array<A, 1> as{{A{}}};
    auto bs = fmap(f, as);
    REQUIRE((std::is_same<decltype(bs)::value_type, B>::value));
    REQUIRE(bs.at(0) == B{});
  }
  SECTION("… std::deque") {
    std::deque as{A{}};
    auto bs = fmap(f, as);
    REQUIRE((std::is_same<decltype(bs)::value_type, B>::value));
    REQUIRE(bs.at(0) == B{});
  }
}

TEST_CASE("Given a function f : A → B, fmap should produce a lambda, which, "
          "when given a standard library sequence container will bind "
          "appropriately to an uncurried fmap variant for …") {
  auto lifted_f = fmap(f);
  SECTION("… std::vector") {
    std::vector as{A{}};
    auto lifted_f = fmap(f);
    REQUIRE((std::is_same<decltype(lifted_f(as))::value_type, B>::value));
    REQUIRE(lifted_f(as).at(0) == B{});
  }
  SECTION("… std::list") {
    std::list as{A{}};
    REQUIRE((std::is_same<decltype(lifted_f(as))::value_type, B>::value));
    REQUIRE(*lifted_f(as).cbegin() == B{});
  }
  SECTION("… std::array") {
    std::array<A, 1> as{{A{}}};
    REQUIRE((std::is_same<decltype(lifted_f(as))::value_type, B>::value));
    REQUIRE(lifted_f(as).at(0) == B{});
  }
  SECTION("… std::deque") {
    std::deque as{A{}};
    REQUIRE((std::is_same<decltype(lifted_f(as))::value_type, B>::value));
    REQUIRE(lifted_f(as).at(0) == B{});
  }
}
