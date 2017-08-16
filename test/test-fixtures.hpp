#pragma once

#include <ostream>
#include <vector>

namespace tst {

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

  // f : A → B
  const auto f = [](A) -> B { return {}; };
  // g : B → C
  const auto g = [](B) -> C { return {}; };
  // h : C → D
  const auto h = [](C) -> D { return {}; };
  // id : T → T
  const auto id = [](auto x) { return x; };

  // Logs which constructor is called by pushing flags into a vector. Also
  // records if an instance was *moved from*!
  struct CtorLogger {

    enum class Flags {
      Default,         // 0
      CopyConstructed, // 1
      CopiedFrom,      // 2
      MoveConstructed, // 3
      MovedFrom        // 4
    };

    std::vector<Flags> flags;
    CtorLogger() : flags{Flags::Default} {}
    CtorLogger(const CtorLogger &orig) : flags{orig.flags} {
      flags.push_back(Flags::CopyConstructed);
    }
    CtorLogger(CtorLogger &&orig) : flags{orig.flags} {
      flags.push_back(Flags::MoveConstructed);
      orig.flags.push_back(Flags::MovedFrom);
    }
  };
} // namespace tst
