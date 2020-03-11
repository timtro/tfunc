// The following tests are not completely orthogonal. Each of the appropriate
// standard container types for which I have defined functors have their functor
// interface tested here.

#include <catch2/catch.hpp>

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
#include "../include/functor/curried-fmap.hpp"
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

TEST_CASE("Given a std::vector<A> …", "[fmap]") {
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

TEST_CASE("For std::vector, calling fmap should induce the same constructor "
          "and assignment calls as simply using std::transform.",
          "[CtorAs], [fmap]") {
  std::vector<CtorLogger> a(2);
  auto b = fmap(id, a);

  std::vector<CtorLogger> ra(2);
  auto rb = std::vector<CtorLogger>{};
  rb.reserve(ra.size()); // If no reserve, you get extra copy ctrn!
  std::transform(ra.cbegin(), ra.cend(), std::back_inserter(rb), id);

  REQUIRE(b[0].flags == rb[0].flags);
  REQUIRE(b[0].flags == b[1].flags);
}

TEST_CASE("The std::list typeconstructor should be a functor:") {
  REQUIRE(tf::is_functor<std::list>::value == true);
}

TEST_CASE("Given a std::list<A> …", "[fmap]") {
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

TEST_CASE("For std::list, calling fmap should induce the same constructor and "
          "assignment calls as simply using std::transform.",
          "[CtorAs], [fmap]") {
  std::list<CtorLogger> a(2);
  auto b = fmap(id, a);

  std::list<CtorLogger> ra(2);
  auto rb = std::list<CtorLogger>{};
  std::transform(ra.cbegin(), ra.cend(), std::back_inserter(rb), id);

  REQUIRE(b.begin()->flags == rb.begin()->flags);
  REQUIRE(b.begin()->flags == (b.begin()++)->flags);
}

TEST_CASE("The std::array typeconstructor will not be a regular functor,  "
          "because it can not be constructed with the form F<A>. For integer "
          "dependant type constructors, is_functor_N should test true:") {
  REQUIRE((tf::is_functor_N<std::array>::value) == true);
}

TEST_CASE("Given a std::array<A,·> …", "[fmap]") {
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

TEST_CASE("For std::array, calling fmap should induce the same constructor and "
          "assignment calls as simply using std::transform, plus a move "
          "construction:",
          "[CtorAs], [fmap]") {
  // NB: Extra move construction is because moving a std::array calls move on
  // each element. So, a move on std::array is O(N) instead of O(1) like the
  // other containers.
  std::array<CtorLogger, 2> a;
  auto b = fmap(id, a);

  std::array<CtorLogger, 2> ra;
  auto rb = std::array<CtorLogger, 2>{};
  std::transform(ra.cbegin(), ra.cend(), rb.begin(), id);

  rb[0].flags.push_back(CtorLogger::MoveConstructed);
  REQUIRE(b[0].flags == rb[0].flags);
  REQUIRE(b[0].flags == b[1].flags);
}

TEST_CASE("The std::deque typeconstructor should be a functor:") {
  REQUIRE(tf::is_functor<std::deque>::value == true);
}

TEST_CASE("Given a std::deque<A> …", "[fmap]") {
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

TEST_CASE("For std::deque, calling fmap should induce the same constructor "
          "and assignment calls as simply using std::transform.",
          "[CtorAs], [fmap]") {
  std::deque<CtorLogger> a(2);
  auto b = fmap(id, a);

  std::deque<CtorLogger> ra(2);
  auto rb = std::deque<CtorLogger>{};
  std::transform(ra.cbegin(), ra.cend(), std::back_inserter(rb), id);

  REQUIRE(b[0].flags == rb[0].flags);
  REQUIRE(b[0].flags == b[1].flags);
}

TEST_CASE("Given a function f : A → B, fmap should produce a function object "
          "which can bind to multiple functor types:",
          "[fmap], [curried]") {
  auto lifted_f = fmap(f);
  SECTION("… std::vector") {
    std::vector as{A{}};
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
