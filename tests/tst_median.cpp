#include <le-cpp-utils/median.h>
#include <catch2/catch_test_macros.hpp>
#include <vector>

TEST_CASE("median")
{
    SECTION("odd count")
    {
        std::vector<int> v = {3, 1, 4, 1, 5};
        REQUIRE(le::median(v.begin(), v.end()) == 3.0f);
    }

    SECTION("even count")
    {
        std::vector<int> v = {1, 2, 3, 4};
        REQUIRE(le::median(v.begin(), v.end()) == 2.5f);
    }

    SECTION("empty throws")
    {
        std::vector<int> v;
        REQUIRE_THROWS_AS(le::median(v.begin(), v.end()), le::median_of_empty_list_exception);
    }
}
