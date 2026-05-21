#include <le-cpp-utils/pairhash.h>
#include <le-cpp-utils/time.h>
#include <catch2/catch_test_macros.hpp>
#include <unordered_map>
#include <chrono>

TEST_CASE("pair_hash")
{
    std::unordered_map<std::pair<int, int>, int, pair_hash> map;
    map[{1, 2}] = 42;
    map[{3, 4}] = 99;

    REQUIRE(map.at({1, 2}) == 42);
    REQUIRE(map.at({3, 4}) == 99);
    REQUIRE(map.find({5, 6}) == map.end());
}

TEST_CASE("time utilities")
{
    SECTION("timeMsSinceEpoch returns positive value")
    {
        REQUIRE(le::timeMsSinceEpoch() > 0);
    }

    SECTION("timeIso8601Utc returns valid format")
    {
        auto now = std::chrono::system_clock::now();
        std::string s = le::timeIso8601Utc(now);
        REQUIRE(s.size() >= 20);
        REQUIRE(s.back() == 'Z');
    }
}
