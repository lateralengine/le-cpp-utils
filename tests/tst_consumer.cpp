#include <le-cpp-utils/consumer.h>
#include <catch2/catch_test_macros.hpp>

using namespace le;

class TestWorker
{
public:
	explicit TestWorker(bool configurePass = true) : m_configurePass(configurePass) {}

	bool configure() { return m_configurePass; }
	void cleanup() {}

	int process(int dataIn) { return ++dataIn; }

private:
	bool m_configurePass;
};

TEST_CASE("Consumer")
{
	SECTION("Consumer no data out")
	{
		{
			Consumer<TestWorker, int> consumer(std::make_unique<TestWorker>(true));
			REQUIRE(consumer.configure());
			consumer.cleanup();
		}

		{
			Consumer<TestWorker, int, int> consumer(std::make_unique<TestWorker>(true));
			REQUIRE(consumer.configure());
			consumer.cleanup();
		}

		{
			Consumer<TestWorker, int> consumer(std::make_unique<TestWorker>(false));
			REQUIRE(!consumer.configure());
			consumer.cleanup();
		}

		{
			Consumer<TestWorker, int> consumer(std::make_unique<TestWorker>(true));
			REQUIRE(consumer.configure());
			consumer.push(0);
			consumer.push(1);
			consumer.cleanup();
		}
	}

	SECTION("Consumer with data out")
	{
		ThreadSafeQueue<int> resultQueue;

		std::function<void(int)> cbDataOut([&](int dataOut) {resultQueue.push(dataOut); });

		{
			
			Consumer<TestWorker, int, int> consumer(std::make_unique<TestWorker>(true), cbDataOut);
			REQUIRE(consumer.configure());
			REQUIRE(!resultQueue.pop());
			consumer.cleanup();
		}

		{
			Consumer<TestWorker, int, int> consumer(std::make_unique<TestWorker>(true), cbDataOut);
			REQUIRE(consumer.configure());
			consumer.push(0);

			int result = resultQueue.wait();
			REQUIRE(result == 1);
			REQUIRE(!resultQueue.pop());

			consumer.push(5);
			consumer.push(10);

			REQUIRE(resultQueue.wait() == 6);
			REQUIRE(resultQueue.wait() == 11);
			REQUIRE(!resultQueue.pop());

			consumer.cleanup();
		}
	}
}