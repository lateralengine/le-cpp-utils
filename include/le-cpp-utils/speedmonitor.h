#include <atomic>
#include <thread>
#include <chrono>
#include <functional>
#include <iostream>
#include <cmath>

namespace le
{

    template<typename T>
    class SpeedMonitor
    {
    public:
        using Callback = std::function<void()>;

        SpeedMonitor(
            std::chrono::milliseconds interval,
            double smoothingFactor,
			double speedThreshold,
            Callback cbRisingEdge,
            Callback cbFallingEdge
        ) :
            pollInterval(interval),
            smoothingFactor(smoothingFactor),
            speedThreshold(speedThreshold),
            cbRisingEdge(cbRisingEdge),
            cbFallingEdge(cbFallingEdge),
            running(true)
        {
            monitorThread = std::thread(&SpeedMonitor::run, this);
        }

		void setValue(T value)
		{
			sharedValue.store(value, std::memory_order_relaxed);
		}

        ~SpeedMonitor()
        {
            running = false;
            if (monitorThread.joinable())
            {
                monitorThread.join();
            }
        }

    private:
        void run()
        {
            using namespace std::chrono;
            T lastValue = sharedValue.load(std::memory_order_relaxed);
            auto lastTime = steady_clock::now();
            double speed = 0.0;

            while (running.load(std::memory_order_relaxed))
            {
                std::this_thread::sleep_for(pollInterval);

				bool previouslyBelowThreshold = speed < speedThreshold;

                T currentValue = sharedValue.load(std::memory_order_relaxed);
                auto currentTime = steady_clock::now();

                // Calculate smoothed speed
                duration<double> dt = currentTime - lastTime;
                double delta = static_cast<double>(currentValue - lastValue);
                double rawSpeed = delta / dt.count(); // units/sec
                speed = smoothingFactor * rawSpeed + (1.0 - smoothingFactor) * speed;

                // Check crossing thresholds
				bool nowBelowThreshold = speed < speedThreshold;
				if (previouslyBelowThreshold && !nowBelowThreshold)
				{
					cbRisingEdge();
				}
				else if (!previouslyBelowThreshold && nowBelowThreshold)
				{
					cbFallingEdge();
				}

                lastValue = currentValue;
                lastTime = currentTime;
            }
        }

        std::atomic<T> sharedValue = 0;
        const std::chrono::milliseconds pollInterval;
        const double smoothingFactor;
        const double speedThreshold;
        Callback cbRisingEdge;
        Callback cbFallingEdge;

        std::atomic<bool> running;
        std::thread monitorThread;
    };

} // namespace le