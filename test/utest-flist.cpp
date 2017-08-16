// The following tests are not completely orthogonal. Each of the appropriate
// standard container types for which I have defined functors have their functor
// interface tested here.

#include <catch/catch.hpp>

#include "test-fixtures.hpp"
using tst::A;  // Tag for unit type
using tst::B;  // Tag for unit type
using tst::C;  // Tag for unit type
using tst::D;  // Tag for unit type
using tst::f;  // f : A → B
using tst::g;  // g : B → C
using tst::h;  // h : C → D
using tst::id; // id : T → T

#include "../include/function-operations.hpp"
#include "../include/functor/flist.hpp"
using tf::as_functor; // as_functor : G<A> → F<A>;
using tf::fmap;       // fmap : (A → B) → F<A> → F<B>

#include <array>
#include <deque>
#include <list>
#include <type_traits>
#include <valarray>
#include <vector>

TEST_CASE("The std::vector type constructor should be a functor:") {
  REQUIRE(tf::is_functor<std::vector>::value == true);
}

TEST_CASE("Given a std::vector<A> …") {
  std::vector<A> as{A{}};

  SECTION("… and a function f : A → B, fmap should produce a std::vector<B>:") {
    auto bs = fmap(f, as);
    REQUIRE((std::is_same<decltype(bs)::value_type, B>::value));
    REQUIRE(bs.at(0) == B{});
  }

  SECTION("… fmap given only a function f : A → B, should produce a function "
          "object, which when subsequently a std::vector<A> will bind "
          "appropriately to an uncurried fmap variant.") {
    auto lifted_f = fmap(f);
    REQUIRE((std::is_same<decltype(lifted_f(as))::value_type, B>::value));
    REQUIRE(lifted_f(as).at(0) == B{});
  }

  SECTION("… mapping over it with the id-function should leave it unchanged") {
    REQUIRE(fmap(id, as).at(0) == A{});
  }

  SECTION("… it should satisfy the composition relation: fmap(g, fmap(f, a)) "
          "== fmap(g∘f, a)") {
    REQUIRE(fmap(g, fmap(f, as)).at(0) == C{});
    REQUIRE(fmap(g, fmap(f, as)).at(0) == fmap(tf::compose(g, f), as).at(0));
  }

  SECTION("… it should cast to a functor:") {
    REQUIRE(as_functor(as).fmap(f).at(0) == B{});
  }
}

TEST_CASE("The std::list typeconstructor should be a functor:") {
  REQUIRE(tf::is_functor<std::list>::value == true);
}

TEST_CASE("Given a std::list<A> …") {
  std::list<A> as{A{}};

  SECTION("… and a function f : A → B, fmap should produce a std::list<B>:") {
    auto bs = fmap(f, as);
    REQUIRE((std::is_same<decltype(bs)::value_type, B>::value));
    REQUIRE(*bs.begin() == B{});
  }

  SECTION("… fmap given only a function f : A → B, should produce a function "
          "object, which when subsequently a std::list<A> will bind "
          "appropriately to an uncurried fmap variant.") {
    auto lifted_f = fmap(f);
    REQUIRE((std::is_same<decltype(lifted_f(as))::value_type, B>::value));
    REQUIRE(*lifted_f(as).begin() == B{});
  }

  SECTION("… mapping over it with the id-function should leave it unchanged") {
    REQUIRE(*fmap(id, as).begin() == A{});
  }

  SECTION("… it should satisfy the composition relation: fmap(g, fmap(f, a)) "
          "== fmap(g∘f, a)") {
    REQUIRE(*fmap(g, fmap(f, as)).begin() == C{});
    REQUIRE(*fmap(g, fmap(f, as)).begin() ==
            *fmap(tf::compose(g, f), as).begin());
  }

  SECTION("… it should cast to a functor:") {
    REQUIRE(*as_functor(as).fmap(f).begin() == B{});
  }
}

TEST_CASE("The std::array typeconstructor should be a functor:") {
  REQUIRE(tf::is_functor<std::list>::value == true);
}

TEST_CASE("Given a std::array<A,·> …") {
  std::array<A, 1> as{{A{}}};

  SECTION("… and a function f : A → B, fmap should produce a std::array<B>:") {
    auto bs = fmap(f, as);
    REQUIRE((std::is_same<decltype(bs)::value_type, B>::value));
    REQUIRE(bs.at(0) == B{});
  }

  SECTION("… fmap given only a function f : A → B, should produce a function "
          "object, which when subsequently a std::array<A> will bind "
          "appropriately to an uncurried fmap variant.") {
    auto lifted_f = fmap(f);
    REQUIRE((std::is_same<decltype(lifted_f(as))::value_type, B>::value));
    REQUIRE(lifted_f(as).at(0) == B{});
  }

  SECTION("… mapping over it with the id-function should leave it unchanged") {
    REQUIRE(fmap(id, as).at(0) == A{});
  }
  SECTION("… it should satisfy the composition relation: fmap(g, fmap(f, a)) "
          "== fmap(g∘f, a)") {
    REQUIRE(fmap(g, fmap(f, as)).at(0) == C{});
    REQUIRE(fmap(g, fmap(f, as)).at(0) == fmap(tf::compose(g, f), as).at(0));
  }

  SECTION("… it should cast to a functor:") {
    REQUIRE(as_functor(as).fmap(f).at(0) == B{});
  }
}

TEST_CASE("The std::deque typeconstructor should be a functor:") {
  REQUIRE(tf::is_functor<std::list>::value == true);
}

TEST_CASE("Given a std::deque<A> …") {
  std::deque<A> as{A{}};

  SECTION("… and a function f : A → B, fmap should produce a std::deque<B>:") {
    auto bs = fmap(f, as);
    REQUIRE((std::is_same<decltype(bs)::value_type, B>::value));
    REQUIRE(bs.at(0) == B{});
  }

  SECTION("… fmap given only a function f : A → B, should produce a function "
          "object, which when subsequently a std::deque<A> will bind "
          "appropriately to an uncurried fmap variant.") {
    auto lifted_f = fmap(f);
    REQUIRE((std::is_same<decltype(lifted_f(as))::value_type, B>::value));
    REQUIRE(lifted_f(as).at(0) == B{});
  }

  SECTION("… mapping over it with the id-function should leave it unchanged") {
    REQUIRE(fmap(id, as).at(0) == A{});
  }

  SECTION("… it should satisfy the composition relation: fmap(g, fmap(f, a)) "
          "== fmap(g∘f, a)") {
    REQUIRE(fmap(g, fmap(f, as)).at(0) == C{});
    REQUIRE(fmap(g, fmap(f, as)).at(0) == fmap(tf::compose(g, f), as).at(0));
  }

  SECTION("… it should cast to a functor:") {
    REQUIRE(as_functor(as).fmap(f).at(0) == B{});
  }
}

TEST_CASE("Given a function f : A → B, fmap should produce a function object "
          "which can bind to multiple functor types:") {
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
