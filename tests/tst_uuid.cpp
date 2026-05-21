#include <le-cpp-utils/uuid_v4.h>
#include <catch2/catch_test_macros.hpp>
#include <random>

TEST_CASE("UUID")
{
    UUIDv4::UUIDGenerator<std::mt19937_64> gen;

    SECTION("string format")
    {
        std::string s = gen.getUUID().str();
        REQUIRE(s.size() == 36);
        REQUIRE(s[8] == '-');
        REQUIRE(s[13] == '-');
        REQUIRE(s[18] == '-');
        REQUIRE(s[23] == '-');
    }

    SECTION("unique generation")
    {
        REQUIRE(gen.getUUID() != gen.getUUID());
    }

    SECTION("string roundtrip")
    {
        auto uuid = gen.getUUID();
        std::string s = uuid.str();
        REQUIRE(UUIDv4::UUID::fromStrFactory(s).str() == s);
    }
}
