#include <ostream>

// clang-format off
struct A {bool operator==(const A) const { return true; }};
struct B {bool operator==(const B) const { return true; }};
struct C {bool operator==(const C) const { return true; }};
struct D {bool operator==(const D) const { return true; }};
// clang-format on

// A nicety so that Catch can print
std::ostream &operator<<(std::ostream &os, A const &) {
  os << "A";
  return os;
}
std::ostream &operator<<(std::ostream &os, B const &) {
  os << "B";
  return os;
}
std::ostream &operator<<(std::ostream &os, C const &) {
  os << "C";
  return os;
}
std::ostream &operator<<(std::ostream &os, D const &) {
  os << "D";
  return os;
}
