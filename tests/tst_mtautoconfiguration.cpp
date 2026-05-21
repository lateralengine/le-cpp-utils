#include <le-cpp-utils/mtautoconfiguration.h>
#include <le-cpp-utils/threadsafequeue.h>
#include <catch2/catch_test_macros.hpp>

using namespace le;

TEST_CASE("MtAutoConfiguration")
{
	SECTION("Simple No Heartbeat")
	{
		ThreadSafeQueue<int> configQueue;
		ThreadSafeQueue<int> stateQueue;

		MtAutoConfiguration mac(
			[&]()
			{
				configQueue.push(1);
				return true;
			},
			[&]() {}
		);
		mac.setCbStateChanged([&](bool value)
			{
			stateQueue.push(value ? 1 : 0);
		});

		mac.start();
		REQUIRE(configQueue.wait() == 1);
		REQUIRE(stateQueue.wait() == 1);
		REQUIRE(configQueue.size() == 0);
		REQUIRE(stateQueue.size() == 0);
		mac.stop();
		REQUIRE(stateQueue.wait() == 0);

		REQUIRE(configQueue.size() == 0);
		REQUIRE(stateQueue.size() == 0);

		mac.start();
		REQUIRE(configQueue.wait() == 1);
		REQUIRE(stateQueue.wait() == 1);
		mac.stop();
		REQUIRE(stateQueue.wait() == 0);

		REQUIRE(configQueue.size() == 0);
		REQUIRE(stateQueue.size() == 0);
	}

	SECTION("Configuration keeps failing")
	{
		ThreadSafeQueue<int> configQueue;
		ThreadSafeQueue<int> stateQueue;

		MtAutoConfiguration mac(
			[&]()
			{
				configQueue.push(1);
				return false;
			},
			[&]() {}
		);
		mac.setCbStateChanged([&](bool value) { stateQueue.push(value ? 1 : 0); });
		mac.setReconfigurationDelay(std::chrono::milliseconds(100));

		mac.start();
		REQUIRE(configQueue.wait() == 1);
		REQUIRE(stateQueue.size() == 0);
		for (int i = 0; i < 3; ++i)
			REQUIRE(configQueue.wait() == 1);
		mac.stop();
		REQUIRE(configQueue.size() == 0);
		REQUIRE(stateQueue.size() == 0);
	}

	SECTION("Request Reconfiguration")
	{
		ThreadSafeQueue<int> configQueue;
		ThreadSafeQueue<int> stateQueue;

		MtAutoConfiguration mac(
			[&]()
			{
				configQueue.push(1);
				return true;
			},
			[&]() {}
		);
		mac.setCbStateChanged([&](bool value) { stateQueue.push(value ? 1 : 0); });
		mac.setReconfigurationDelay(std::chrono::milliseconds(100));

		mac.start();
		REQUIRE(configQueue.wait() == 1);
		REQUIRE(stateQueue.wait() == 1);
		REQUIRE(configQueue.size() == 0);
		REQUIRE(stateQueue.size() == 0);

		auto lock = mac.acquireResourceLock();
		REQUIRE(mac.ok() == true);
		mac.requestReconfiguration();
		REQUIRE(mac.ok() == false);
		lock.unlock();

		REQUIRE(stateQueue.wait() == 0);

		REQUIRE(configQueue.wait() == 1);
		REQUIRE(stateQueue.wait() == 1);
		REQUIRE(configQueue.size() == 0);
		REQUIRE(stateQueue.size() == 0);

		mac.stop();

		REQUIRE(stateQueue.wait() == 0);
		REQUIRE(configQueue.size() == 0);
		REQUIRE(stateQueue.size() == 0);
	}

	SECTION("Heartbeat OK")
	{
		ThreadSafeQueue<int> configQueue;
		ThreadSafeQueue<int> hbQueue;
		ThreadSafeQueue<int> stateQueue;

		MtAutoConfiguration mac(
			[&]()
			{
				configQueue.push(1);
				return true;
			},
			[&]() {}
		);
		mac.setCbStateChanged([&](bool value) { stateQueue.push(value ? 1 : 0); });
		mac.setReconfigurationDelay(std::chrono::milliseconds(2000));
		mac.setFnHeartbeat(
			[&](bool)
			{
				hbQueue.push(1);
				return true;
			}
		);
		mac.setHeartbeatTimeout(std::chrono::milliseconds(100));

		mac.start();
		REQUIRE(configQueue.wait() == 1);
		REQUIRE(stateQueue.wait() == 1);
		REQUIRE(configQueue.size() == 0);
		REQUIRE(stateQueue.size() == 0);

		for (int i = 0; i < 3; ++i)
			REQUIRE(hbQueue.wait() == 1);

		REQUIRE(configQueue.size() == 0);
		REQUIRE(stateQueue.size() == 0);

		mac.stop();

		REQUIRE(stateQueue.wait() == 0);
		REQUIRE(configQueue.size() == 0);
		REQUIRE(stateQueue.size() == 0);
	}

	SECTION("Heartbeat Fail")
	{
		ThreadSafeQueue<int> configQueue;
		ThreadSafeQueue<int> hbQueue;
		ThreadSafeQueue<int> stateQueue;

		MtAutoConfiguration mac(
			[&]()
			{
				configQueue.push(1);
				return true;
			},
			[&]() {}
		);
		mac.setCbStateChanged([&](bool value) { stateQueue.push(value ? 1 : 0); });
		mac.setReconfigurationDelay(std::chrono::milliseconds(100));
		mac.setFnHeartbeat(
			[&](bool)
			{
				hbQueue.push(0);
				return false;
			}
		);
		mac.setHeartbeatTimeout(std::chrono::milliseconds(100));

		mac.start();
		REQUIRE(configQueue.wait() == 1);
		REQUIRE(hbQueue.wait() == 0);

		REQUIRE(stateQueue.size() == 0);

		REQUIRE(configQueue.wait() == 1);
		REQUIRE(hbQueue.wait() == 0);

		REQUIRE(stateQueue.size() == 0);

		mac.stop();

		REQUIRE(stateQueue.size() == 0);
	}

	SECTION("Heartbeat 2nd fail")
	{
		ThreadSafeQueue<int> configQueue;
		ThreadSafeQueue<int> hbQueue;
		ThreadSafeQueue<int> stateQueue;
		int hbCount = 0;

		MtAutoConfiguration mac(
			[&]()
			{
				configQueue.push(1);
				return true;
			},
			[&]() {}
		);
		mac.setCbStateChanged([&](bool value) { stateQueue.push(value ? 1 : 0); });
		mac.setReconfigurationDelay(std::chrono::milliseconds(100));
		mac.setFnHeartbeat(
			[&](bool)
			{
				if (hbCount++ == 0)
				{
					hbQueue.push(1);
					return true;
				}
				else
				{
					hbQueue.push(0);
					return false;
				}
			}
		);
		mac.setHeartbeatTimeout(std::chrono::milliseconds(100));

		mac.start();
		REQUIRE(configQueue.wait() == 1);
		REQUIRE(stateQueue.wait() == 1);
		REQUIRE(hbQueue.wait() == 1);

		REQUIRE(hbQueue.wait() == 0);
		REQUIRE(stateQueue.wait() == 0);

		REQUIRE(configQueue.wait() == 1);
		REQUIRE(hbQueue.wait() == 0);
		REQUIRE(stateQueue.size() == 0);

		mac.stop();

		REQUIRE(stateQueue.size() == 0);
	}

	SECTION("User Locked")
	{
		ThreadSafeQueue<int> configQueue;
		ThreadSafeQueue<int> hbQueue;
		ThreadSafeQueue<int> stateQueue;

		MtAutoConfiguration mac(
			[&]()
			{
				configQueue.push(1);
				return true;
			},
			[&]() {}
		);
		mac.setCbStateChanged([&](bool value) { stateQueue.push(value ? 1 : 0); });
		mac.setReconfigurationDelay(std::chrono::milliseconds(1));
		mac.setFnHeartbeat(
			[&](bool)
			{
				hbQueue.push(1);
				return true;
			}
		);
		mac.setHeartbeatTimeout(std::chrono::milliseconds(1));

		mac.start();
		REQUIRE(configQueue.wait() == 1);
		REQUIRE(stateQueue.wait() == 1);

		{
			auto lock = mac.acquireResourceLock();
			while (hbQueue.size() != 0)
				hbQueue.pop();
			REQUIRE(hbQueue.size() == 0);
			REQUIRE(mac.ok() == true);
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
			REQUIRE(hbQueue.size() == 0);
		}

		REQUIRE(hbQueue.wait() == 1);

		mac.stop();

		REQUIRE(stateQueue.wait() == 0);
		REQUIRE(stateQueue.size() == 0);
	}
}