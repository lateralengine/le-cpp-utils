#include <le-cpp-utils/threadsafedata.h>
#include <le-cpp-utils/threadsafeevent.h>
#include <le-cpp-utils/threadinterval.h>
#include <catch2/catch_test_macros.hpp>
#include <atomic>
#include <thread>

using namespace std::chrono_literals;

TEST_CASE("ThreadSafeData")
{
    SECTION("default value")
    {
        le::ThreadSafeData<int> data;
        REQUIRE(data.get() == 0);
    }

    SECTION("set and get round-trip")
    {
        le::ThreadSafeData<int> data;
        data.set(42);
        REQUIRE(data.get() == 42);
    }
}

TEST_CASE("ThreadSafeEvent")
{
    SECTION("wait_for times out when not set")
    {
        le::ThreadSafeEvent event;
        REQUIRE_FALSE(event.wait_for(5ms));
    }

    SECTION("wait_for returns true after set")
    {
        le::ThreadSafeEvent event;
        event.set();
        REQUIRE(event.wait_for(100ms));
    }
}

TEST_CASE("ThreadInterval")
{
    SECTION("callback fires at least once")
    {
        std::atomic<int> count = 0;
        {
            le::ThreadInterval interval(50ms, [&] { ++count; });
            std::this_thread::sleep_for(200ms);
        }
        REQUIRE(count >= 1);
    }
}
