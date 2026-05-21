#include <le-cpp-utils/versioncompare.h>
#include <catch2/catch_test_macros.hpp>

TEST_CASE("LessThanVersion")
{
    REQUIRE(le::LessThanVersion("1.0.0.0", "2.0.0.0"));
    REQUIRE_FALSE(le::LessThanVersion("2.0.0.0", "1.0.0.0"));
    REQUIRE_FALSE(le::LessThanVersion("1.0.0.0", "1.0.0.0"));
    REQUIRE(le::LessThanVersion("1.0.0.9", "1.0.1.0"));
    REQUIRE(le::LessThanVersion("1.2.3.4", "1.2.3.5"));
}
