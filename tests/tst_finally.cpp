#include <le-cpp-utils/finally.h>
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Finally")
{
    int callCount = 0;

    SECTION("callback invoked on scope exit")
    {
        {
            le::Finally f([&] { ++callCount; });
            REQUIRE(callCount == 0);
        }
        REQUIRE(callCount == 1);
    }

    SECTION("callback not invoked before scope exit")
    {
        le::Finally f([&] { ++callCount; });
        REQUIRE(callCount == 0);
    }
}
