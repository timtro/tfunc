#include "../include/function-composition-and-currying.hpp"

#include <catch/catch.hpp>
#include <ostream>
#include <string>

using tf::compose;
using tf::id;

struct TypeA {
  bool operator==(const TypeA) const { return true; }
};
struct TypeB {
  bool operator==(const TypeB) const { return true; }
};
struct TypeC {
  bool operator==(const TypeC) const { return true; }
};
struct TypeD {
  bool operator==(const TypeD) const { return true; }
};

// f : A → B
auto f = [](TypeA) -> TypeB { return {}; };
// g : B → C
auto g = [](TypeB) -> TypeC { return {}; };
// h : A → B
auto h = [](TypeC) -> TypeD { return {}; };

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

TEST_CASE("`compose(f)` == f.id_A == id_B.f.", "[mathematical]") {
  REQUIRE(compose(f)(TypeA{}) == compose(f, id<TypeA>)(TypeA{}));
  REQUIRE(compose(f, id<TypeA>)(TypeA{}) == compose(id<TypeB>, f)(TypeA{}));
}

TEST_CASE("Check associativity: (h.g).f == h.(g.f)", "[mathematical]") {
  REQUIRE(TypeD{} == compose(h, g, f)(TypeA{}));
  REQUIRE(compose(h, g, f)(TypeA{}) == compose(h, compose(g, f))(TypeA{}));
  REQUIRE(compose(compose(h, g), f)(TypeA{}) ==
          compose(h, compose(g, f))(TypeA{}));
}

TEST_CASE("Compose two C-functions", "[interface]") {
  auto id2 = compose(id<TypeA>, id<TypeA>);
  REQUIRE(id2(TypeA{}) == TypeA{});
}

template <typename T>
struct Getable {
  T N;
  Getable(T N) : N(N) {}
  T get() const { return N; }
};

TEST_CASE("Sould be able to compose with PMFs", "[interface]") {
  const auto a = TypeA{};
  Getable getable_a{a};
  auto fog = compose(id<TypeA>, &Getable<TypeA>::get);
  REQUIRE(fog(&getable_a) == a);
}
