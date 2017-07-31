#include "../include/function-composition-and-currying.hpp"

#include <catch/catch.hpp>
#include <ostream>
#include <string>

using tf::compose;
using tf::id;
using tf::pipe;

// clang-format off
struct TypeA {bool operator==(const TypeA) const { return true; }};
struct TypeB {bool operator==(const TypeB) const { return true; }};
struct TypeC {bool operator==(const TypeC) const { return true; }};
struct TypeD {bool operator==(const TypeD) const { return true; }};
// clang-format on

// f : A → B
auto f = [](TypeA) -> TypeB { return {}; };
// g : B → C
auto g = [](TypeB) -> TypeC { return {}; };
// h : C → D
auto h = [](TypeC) -> TypeD { return {}; };

// A nicety so that Catch can print
std::ostream &operator<<(std::ostream &os, TypeA const &) {
  os << "TypeA";
  return os;
}
std::ostream &operator<<(std::ostream &os, TypeB const &) {
  os << "TypeB";
  return os;
}
std::ostream &operator<<(std::ostream &os, TypeC const &) {
  os << "TypeC";
  return os;
}
std::ostream &operator<<(std::ostream &os, TypeD const &) {
  os << "TypeD";
  return os;
}

TEST_CASE("`compose(f)` == f.id_A == id_B.f.", "[compose], [mathematical]") {
  REQUIRE(compose(f)(TypeA{}) == compose(f, id<TypeA>)(TypeA{}));
  REQUIRE(compose(f, id<TypeA>)(TypeA{}) == compose(id<TypeB>, f)(TypeA{}));
}

TEST_CASE("Check associativity: (h.g).f == h.(g.f)",
          "[compose], [mathematical]") {
  REQUIRE(TypeD{} == compose(h, g, f)(TypeA{}));
  REQUIRE(compose(h, g, f)(TypeA{}) == compose(h, compose(g, f))(TypeA{}));
  REQUIRE(compose(compose(h, g), f)(TypeA{}) ==
          compose(h, compose(g, f))(TypeA{}));
}

TEST_CASE("Compose two C-functions", "[compose], [interface]") {
  auto id2 = compose(id<TypeA>, id<TypeA>);
  REQUIRE(id2(TypeA{}) == TypeA{});
}

template <typename T>
struct Getable {
  T N;
  Getable(T N) : N(N) {}
  T get() const { return N; }
};

TEST_CASE("Sould be able to compose with PMFs", "[compose], [interface]") {
  const auto a = TypeA{};
  Getable getable_a{a};
  auto fog = compose(id<TypeA>, &Getable<TypeA>::get);
  REQUIRE(fog(&getable_a) == a);
}

TEST_CASE("Piping should bear a relationship to composition, which is "
          "basically an order reversal in the argument list, with the "
          "inclusion of an initial argument.",
          "[compose], [pipe], [mathematical]") {
  REQUIRE(compose(h, g, f)(TypeA{}) == pipe(TypeA{}, f, g, h));
}

TEST_CASE("`pipe(f)` == `pipe(a, id_A, F)`` == `pipe(b, f id_B)`.",
          "[pipe], [mathematical]") {
  REQUIRE(pipe(TypeA{}, f) == pipe(TypeA{}, id<TypeA>, f));
  REQUIRE(pipe(TypeA{}, id<TypeA>, f) == pipe(TypeA{}, f, id<TypeB>));
}

TEST_CASE("Should work with C-functions", "[pipe], [interface]") {
  REQUIRE(pipe(TypeA{}, id<TypeA>, id<TypeA>) == TypeA{});
}
