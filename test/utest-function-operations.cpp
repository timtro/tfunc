#include <catch2/catch.hpp>

#include "test-fixtures.hpp"
using tst::A; // Tag for unit type
using tst::B; // Tag for unit type
using tst::C; // Tag for unit type
using tst::CtorLogger;
using tst::D; // Tag for unit type
using tst::f; // f : A → B
using tst::g; // g : B → C
using tst::h; // h : C → D

#include "../include/function-operations.hpp"

#include <type_traits>

using tf::compose;
using tf::curry;
using tf::curry···;
using tf::id;
using tf::pipe;

TEST_CASE("Polymorphic identity function should perfectly forward and …",
          "[id], [interface]") {
  SECTION("… identify lvalues", "[mathematical]") {
    auto a = A{};
    auto b = B{};
    REQUIRE(id(a) == A{});
    REQUIRE(id(b) == B{});
  }
  SECTION("… preserve lvalue-refness") {
    REQUIRE(
        std::is_lvalue_reference<decltype(id(std::declval<int &>()))>::value);
  }
  SECTION("… identify rvalues", "[mathematical]") {
    REQUIRE(id(A{}) == A{});
    REQUIRE(id(B{}) == B{});
  }
  SECTION("… preserve rvalue-refness") {
    REQUIRE(
        std::is_rvalue_reference<decltype(id(std::declval<int &&>()))>::value);
  }
}

TEST_CASE("`compose(f)` == f.id_A == id_B.f.", "[compose], [mathematical]") {
  REQUIRE(compose(f)(A{}) == compose(f, id<A>)(A{}));
  REQUIRE(compose(f, id<A>)(A{}) == compose(id<B>, f)(A{}));
}

TEST_CASE("Check associativity: (h.g).f == h.(g.f)",
          "[compose], [mathematical]") {
  REQUIRE(D{} == compose(h, g, f)(A{}));
  REQUIRE(compose(h, g, f)(A{}) == compose(h, compose(g, f))(A{}));
  REQUIRE(compose(compose(h, g), f)(A{}) == compose(h, compose(g, f))(A{}));
}

TEST_CASE("Compose two C-functions", "[compose], [interface]") {
  auto id2 = compose(id<A>, id<A>);
  REQUIRE(id2(A{}) == A{});
}

template <typename T>
struct Getable {
  T N;
  Getable(T N) : N(N) {}
  T get() const { return N; }
};

TEST_CASE("Sould be able to compose with PMFs", "[compose], [interface]") {
  const auto a = A{};
  Getable getable_a{a};
  auto fog = compose(id<A>, &Getable<A>::get);
  REQUIRE(fog(&getable_a) == a);
}

TEST_CASE("Return of a composed function should preserve the rvalue-refness of "
          "the outer function",
          "[compose], [interface]") {
  B b{};
  auto ref_to_b = [&b](A) -> B & { return b; };
  auto fog = compose(ref_to_b, id<A>);
  REQUIRE(std::is_lvalue_reference<decltype(fog(A{}))>::value);
}

TEST_CASE("Piping should bear a relationship to composition, which is "
          "basically an order reversal in the argument list, with the "
          "inclusion of an initial argument.",
          "[compose], [pipe], [mathematical]") {
  REQUIRE(compose(h, g, f)(A{}) == pipe(A{}, f, g, h));
}

TEST_CASE("`pipe(f)` == `pipe(a, id_A, F)`` == `pipe(b, f id_B)`.",
          "[pipe], [mathematical]") {
  REQUIRE(pipe(A{}, f) == pipe(A{}, id<A>, f));
  REQUIRE(pipe(A{}, id<A>, f) == pipe(A{}, f, id<B>));
}

TEST_CASE("Pipes should work with C-functions", "[pipe], [interface]") {
  REQUIRE(pipe(A{}, id<A>, id<A>) == A{});
}

TEST_CASE(
    "Return of a pipe should preserve the rvalue-refness of the outer function",
    "[compose], [interface]") {
  B b{};
  auto ref_to_b = [&b](A) -> B & { return b; };
  REQUIRE(std::is_lvalue_reference<decltype(pipe(A{}, ref_to_b))>::value);
}

TEST_CASE("Curried non-variadic functions should bind arguments, one at a "
          "time, from left to right.",
          "[curry], [non-variadic], [interface]") {

  // abcd : (A, B, C, D) → (A, B, C, D)
  // a_b_c_d : A → B → C → D → (A, B, C, D)
  auto abcd = [](A a, B b, C c, D d) { return std::tuple{a, b, c, d}; };
  auto a_b_c_d = curry(abcd);

  auto[a, b, c, d] = a_b_c_d(A{})(B{})(C{})(D{});
  REQUIRE(a == A{});
  REQUIRE(b == B{});
  REQUIRE(c == C{});
  REQUIRE(d == D{});
}

TEST_CASE("Curried variadic functions should bind arguments, one at a time, "
          "from left to right untill a call-object is passed.",
          "[curry], [variadic], [interface]") {
  // argument_echo : (Q → W → …) → (Q, W, …), for any types Q, W, ….
  const auto argument_echo = [](auto... xs) {
    return std::tuple<decltype(xs)...>{xs...};
  };

  // argument_echo··· : Q → W → … → (Q, W, …), for any types Q, W, ….
  auto argument_echo··· = curry···(argument_echo);
  auto[a, b, c, d] = argument_echo···(A{})(B{})(C{})(D{})(tf::call);
  REQUIRE(a == A{});
  REQUIRE(b == B{});
  REQUIRE(c == C{});
  REQUIRE(d == D{});
}

D ABC_to_D(A, B, C) { return {}; }

TEST_CASE("C-functions should curry", "[curry], [variadic], [interface]") {

  auto A_B_C_to_D = curry(ABC_to_D);
  REQUIRE(A_B_C_to_D(A{})(B{})(C{}) == D{});
}

struct Foo {
  D d_returner(A, B, C) { return {}; }
};

TEST_CASE("PMFs should curry", "[curry], [non-variadic], [interface]") {
  Foo foo;
  auto foo_d_returner = curry(&Foo::d_returner);
  REQUIRE(foo_d_returner(&foo)(A{})(B{})(C{}) == D{});
  //                     ^ Always give pointer to object first.
}

TEST_CASE("A curried non-variadic function should preserve the lvalue ref-ness "
          "of whatever is returned from the wrapped function.",
          "[curry], [non-variadic], [interface]") {
  A a{};
  auto ref_to_a = [&a]() -> A & { return a; };
  REQUIRE(std::is_lvalue_reference<decltype(curry(ref_to_a))>::value);
}

TEST_CASE("A curried variadic function should preserve the lvalue ref-ness of "
          "whatever is returned from the wrapped function.",
          "[curry], [variadic], [interface]") {
  A a{};
  auto ref_to_a = [&a](...) -> A & { return a; };
  REQUIRE(curry···(ref_to_a)(tf::call) == A{});
  REQUIRE(
      std::is_lvalue_reference<decltype(curry···(ref_to_a)(tf::call))>::value);
}

TEST_CASE("Curried functions should…") {
  auto ABtoC = curry([](A, B) -> C { return {}; });
  auto BtoC = ABtoC(A{});

  SECTION("… compose with other callables.",
          "[curry], [compose], [interface]") {
    REQUIRE(compose(BtoC, f)(A{}) == C{});
  }

  SECTION("… be components of pipeways.", "[curry], [pipe], [interface]") {
    REQUIRE(pipe(A{}, f, BtoC) == C{});
  }
}

TEST_CASE("Currying should work with the C++14 std outfix operators") {
  auto plus = curry(std::plus<int>{});
  auto increment = plus(1);
  REQUIRE(increment(0) == 1);
}
