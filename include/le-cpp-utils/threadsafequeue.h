#pragma once

#include <queue>
#include <mutex>
#include <optional>
#include <chrono>
#include <condition_variable>

namespace le
{

    template<typename T>
    class ThreadSafeQueue {
    public:
        ThreadSafeQueue() = default;
        ThreadSafeQueue(const ThreadSafeQueue<T>&) = delete;
        ThreadSafeQueue& operator=(const ThreadSafeQueue<T>&) = delete;
        ThreadSafeQueue(ThreadSafeQueue<T>&& other)
        {
            std::scoped_lock lock(m_mutex);
            m_queue = std::move(other.m_queue);
        }

        virtual ~ThreadSafeQueue() { }

        size_t size() const
        {
            std::scoped_lock lock(m_mutex);
            return m_queue.size();
        }

        /// <summary>
        /// Pops the next element (if any) from the queue.
        /// </summary>
        /// <returns>Next element in queue or no value if the queue is empty.</returns>
        std::optional<T> pop()
        {
            std::scoped_lock lock(m_mutex);
            if (m_queue.empty())
                return {};
            T tmp = m_queue.front();
            m_queue.pop();
            return tmp;
        }

        /// <summary>
        /// Waits indefinitely until the queue is not empty.
        /// </summary>
        /// <returns>Next element in queue.</returns>
        T wait()
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            m_cv.wait(lock, [this] {return !m_queue.empty(); });
            T tmp = m_queue.front();
            m_queue.pop();
            return tmp;
        }

        /// <summary>
        /// Waits the timeout duration or until the queue is not empty.
        /// </summary>
        /// <returns>Next element in queue or no value if timed out.</returns>
        std::optional<T> wait(std::chrono::steady_clock::duration timeout)
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            if (!m_cv.wait_for(lock, timeout, [this] {return !m_queue.empty(); }))
                return {}; // timed out

            T tmp = m_queue.front();
            m_queue.pop();
            return std::make_optional(tmp);
        }

        /// <summary>
        /// Pushes one item to the queue and notifies one on the condition variable.
        /// </summary>
        void push(const T& item)
        {
            {
                std::scoped_lock lock(m_mutex);
                m_queue.push(item);
            }
            m_cv.notify_one();
        }

        void clear()
        {
            std::scoped_lock lock(m_mutex);
            m_queue = std::queue<T>();
        }

    private:
        std::queue<T> m_queue;
        mutable std::mutex m_mutex;
        std::condition_variable m_cv;
        
    };

} // namespace le