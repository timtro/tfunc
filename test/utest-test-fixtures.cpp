#include <catch/catch.hpp>

#include "test-fixtures.hpp"

using tst::CtorLogger;

constexpr auto Default = CtorLogger::Default;
constexpr auto CopyConstructed = CtorLogger::CopyConstructed;
constexpr auto MoveConstructed = CtorLogger::MoveConstructed;
constexpr auto MovedFrom = CtorLogger::MovedFrom;

TEST_CASE("CtorLogger should properly identify which constructor is called:") {
  CtorLogger default_constructed;
  SECTION("Default constructed should have a history of a default constructed "
          "value:") {
    REQUIRE(default_constructed.flags == std::vector{Default});
  }

  SECTION("Copy constructed should contained the default_constructed flags, "
          "and indicate copy construction:") {
    auto copy_constructed = default_constructed;
    REQUIRE(copy_constructed.flags == (std::vector{Default, CopyConstructed}));
  }

  SECTION("Move constructed object should reflect default construction of "
          "original, and the move construction. Also, the object moved from "
          "should have logged that it was moved from.") {
    CtorLogger to_be_moved; // == CtorLogger::Flags::Default
    auto moved_to = std::move(to_be_moved);
    REQUIRE(to_be_moved.flags == (std::vector{Default, MovedFrom}));
    REQUIRE(moved_to.flags == (std::vector{Default, MoveConstructed}));
  }

  // TODO: Write test for non-consty copy ctor.
}
