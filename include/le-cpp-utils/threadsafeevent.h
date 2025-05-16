#include <future>
#include <chrono>
#include <mutex>

namespace le
{
    /// <summary>
	/// One-shot event that can be set from one thread and waited on from another.
	/// Note that the event can not be reused. A new instance must be created for each new event.
    /// </summary>
    class ThreadSafeEvent
    {
    public:
        ThreadSafeEvent(const ThreadSafeEvent&) = delete;
        ThreadSafeEvent& operator=(const ThreadSafeEvent&) = delete;
        ThreadSafeEvent(ThreadSafeEvent&& other) = delete;
        ThreadSafeEvent() : future(promise.get_future()) {}

        /// <summary>
		/// Sets the event. Threads waiting on this event will be released.
        /// </summary>
        void set()
        {
            promise.set_value();
        }

        /// <summary>
		/// Wait indefinitely for the event to be signaled.
        /// </summary>
        void wait()
        {
            future.wait();
        }

        /// <summary>
		/// Wait for a certain duration for the event to be signaled.
		/// Returns true if the event was signaled, false if the timeout expired.
        /// </summary>
        template <typename Rep, typename Period>
        bool wait_for(const std::chrono::duration<Rep, Period>& timeout)
        {
            return future.wait_for(timeout) == std::future_status::ready;
        }

    private:
        std::promise<void> promise;
        std::future<void> future;
    };

} // namespace le
