#include <le-cpp-utils/threadsafequeue.h>
#include <functional>
#include <catch2/catch_test_macros.hpp>

TEST_CASE("ThreadSafeQueue")
{
	SECTION("ThreadSafeQueue")
	{
		le::ThreadSafeQueue<int> resultQueue;

		std::function<void(int)> cbDataOut([&](int dataOut) {resultQueue.push(dataOut); });

		REQUIRE(!resultQueue.pop());
		REQUIRE(resultQueue.size() == 0);

		resultQueue.push(0);

		REQUIRE(resultQueue.size() == 1);

		auto popped = resultQueue.pop();
		REQUIRE(popped);
		REQUIRE(*popped == 0);

		resultQueue.push(1);
		resultQueue.push(2);

		REQUIRE(resultQueue.size() == 2);

		REQUIRE(resultQueue.wait() == 1);
		REQUIRE(resultQueue.wait() == 2);

		REQUIRE(!resultQueue.wait(std::chrono::milliseconds(1)));

		resultQueue.push(1);
		resultQueue.push(2);

		resultQueue.clear();

		REQUIRE(!resultQueue.pop());
	}
}