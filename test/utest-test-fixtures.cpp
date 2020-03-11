#include <catch2/catch.hpp>

#include "test-fixtures.hpp"

using tst::CtorLogger;

constexpr auto Default = CtorLogger::Default;
constexpr auto CopyConstructed = CtorLogger::CopyConstructed;
constexpr auto NCCopyConstructed = CtorLogger::NCCopyConstructed;
constexpr auto NCCopiedFrom = CtorLogger::NCCopiedFrom;
constexpr auto CopyAssignedTo = CtorLogger::CopyAssignedTo;
constexpr auto NCCopyAssignedTo = CtorLogger::NCCopyAssignedTo;
constexpr auto NCCopyAssignedFrom = CtorLogger::NCCopyAssignedFrom;
constexpr auto MoveConstructed = CtorLogger::MoveConstructed;
constexpr auto MovedFrom = CtorLogger::MovedFrom;
/* constexpr auto MoveAssignedFrom = CtorLogger::MoveAssignedFrom; */
/* constexpr auto MoveAssignedTo = CtorLogger::MoveAssignedTo; */

TEST_CASE("Default constructed should have a history of a default constructed "
          "value:") {
  CtorLogger default_constructed;
  REQUIRE(default_constructed.flags == std::vector{Default});
}

TEST_CASE("In (cost) copy construction, the constructed object should reflect "
          "default construction of original, and the const copy construction. "
          "Also, the object copied from should have no record of being copied "
          "from.") {
  const CtorLogger default_constructed;
  auto copy_constructed = default_constructed;
  REQUIRE(copy_constructed.flags == (std::vector{Default, CopyConstructed}));
  REQUIRE(default_constructed.flags == std::vector{Default});
}

TEST_CASE("In non-cost copy construction, the constructed object should "
          "reflect default construction of original, and the non-const copy "
          "construction. Also, the object copied from should have logged that "
          "it was copied from since the operation was non-const.") {
  CtorLogger default_constructed;
  CtorLogger to_be_copied; // == CtorLogger::Flags::Default
  auto copied_to = to_be_copied;
  REQUIRE(to_be_copied.flags == (std::vector{Default, NCCopiedFrom}));
  REQUIRE(copied_to.flags == (std::vector{Default, NCCopyConstructed}));
}

TEST_CASE("In non-const copy assignment, the copied object should reflect "
          "default construction of its origin, and the non-const copy "
          "assignment. Also, the object copied from should have logged that it "
          "was copy assigned from, since the operation was non-const.") {
  CtorLogger copied_from;
  CtorLogger copied_into;
  copied_into = copied_from;
  REQUIRE(copied_from.flags == (std::vector{Default, NCCopyAssignedFrom}));
  REQUIRE(copied_into.flags == (std::vector{Default, NCCopyAssignedTo}));
}

TEST_CASE("In const copy assignment, the copied object should reflect default "
          "construction of its origin, and the const copy assignment. Also, "
          "the object copied from should have no record of being copied "
          "from.") {
  const CtorLogger copied_from;
  CtorLogger copied_into;
  copied_into = copied_from;
  REQUIRE(copied_from.flags == (std::vector{Default}));
  REQUIRE(copied_into.flags == (std::vector{Default, CopyAssignedTo}));
}

TEST_CASE("In move construction, the constructed object should reflect default "
          "construction of the original, and the move construction. Also, the "
          "object moved from should have logged that it has been moved from.") {
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
