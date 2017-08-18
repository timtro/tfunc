#include <catch/catch.hpp>

#include "test-fixtures.hpp"

using tst::CtorLogger;

constexpr auto Default = CtorLogger::Default;
constexpr auto CopyConstructed = CtorLogger::CopyConstructed;
constexpr auto NCCopyConstructed = CtorLogger::NCCopyConstructed;
constexpr auto NCCopiedFrom = CtorLogger::NCCopiedFrom;
constexpr auto MoveConstructed = CtorLogger::MoveConstructed;
constexpr auto MovedFrom = CtorLogger::MovedFrom;
constexpr auto MoveAssignedFrom = CtorLogger::MoveAssignedFrom;
constexpr auto MoveAssignedTo = CtorLogger::MoveAssignedTo;

TEST_CASE("Default constructed should have a history of a default constructed "
          "value:") {
  CtorLogger default_constructed;
  REQUIRE(default_constructed.flags == std::vector{Default});
}

TEST_CASE("Copy constructed from a const object should contain the  "
          "default_constructed flag, and indicate (const) copy construction "
          "while copied from remains untouched:") {
  const CtorLogger default_constructed;
  auto copy_constructed = default_constructed;
  REQUIRE(copy_constructed.flags == (std::vector{Default, CopyConstructed}));
  REQUIRE(default_constructed.flags == std::vector{Default});
}

TEST_CASE("Copy constructed object should reflect default construction of "
          "original, and the move construction. Also, the object copied from "
          "should have logged that it was copied from, because a compiler will "
          "chose a non-const copy constructor if it isn't needed.") {
  CtorLogger default_constructed;
  CtorLogger to_be_copied; // == CtorLogger::Flags::Default
  auto copied_to = to_be_copied;
  REQUIRE(to_be_copied.flags == (std::vector{Default, NCCopiedFrom}));
  REQUIRE(copied_to.flags == (std::vector{Default, NCCopyConstructed}));
}

TEST_CASE("Move constructed object should reflect default construction of "
          "original, and the move construction. Also, the object moved from "
          "should have logged that it was moved from.") {
  CtorLogger default_constructed;
  CtorLogger to_be_moved; // == CtorLogger::Flags::Default
  auto moved_to = std::move(to_be_moved);
  REQUIRE(to_be_moved.flags == (std::vector{Default, MovedFrom}));
  REQUIRE(moved_to.flags == (std::vector{Default, MoveConstructed}));
}

TEST_CASE("In move assignment, the moved object should reflect default "
          "construction of its origin, and the move assignment. Also, the "
          "object moved from should have logged  that it was move assigned "
          "from.") {
  CtorLogger default_constructed;
  CtorLogger to_be_moved; // == CtorLogger::Flags::Default
  auto moved_to = std::move(to_be_moved);
  REQUIRE(to_be_moved.flags == (std::vector{Default, MovedFrom}));
  REQUIRE(moved_to.flags == (std::vector{Default, MoveConstructed}));
}
