#include <atomic>
#include <thread>
#include <chrono>
#include <functional>

namespace le
{

    template<typename T>
    class ThreadInterval
    {
    public:
        using Callback = std::function<void()>;

        ThreadInterval(
            std::chrono::milliseconds interval,
            Callback callback
        ) :
            interval(interval),
            callback(callback),
            running(true)
        {
            thread = std::thread(&ThreadInterval::run, this);
        }

        ThreadInterval(const ThreadInterval&) = delete;
        ThreadInterval& operator=(const ThreadInterval&) = delete;
        ThreadInterval() = delete;

        ~ThreadInterval()
        {
            running = false;
            if (thread.joinable())
                thread.join();
        }

    private:
        void run()
        {
            using namespace std::chrono;

            while (true)
            {
                std::this_thread::sleep_for(interval);
                if (!running)
                    break;
                callback();
            }
        }

        const std::chrono::milliseconds interval;
        Callback callback;

        std::atomic<bool> running;
        std::thread thread;
    };

} // namespace le