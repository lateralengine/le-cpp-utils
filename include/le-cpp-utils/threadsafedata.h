#pragma once

#include <mutex>

namespace le
{
	/// <summary>
	/// Wraps copyable data so it can be written and read from in multiple threads.
	/// </summary>
    template <typename T>
	class ThreadSafeData
	{
        static_assert(std::is_copy_constructible<T>::value, "ThreadSafeData can only be used with copyable types.");

	public:
        ThreadSafeData() = default;
        ThreadSafeData(const ThreadSafeData<T>&) = delete;
        ThreadSafeData& operator=(const ThreadSafeData<T>&) = delete;
        ThreadSafeData(ThreadSafeData<T>&& other)
        {
            std::scoped_lock lock(mutex);
            data = std::move(other.data);
        }

		void set(const T& data)
        {
            std::lock_guard<std::mutex> lock(mutex);
            this->data = data;
        }

        T get() const
        {
            std::lock_guard<std::mutex> lock(mutex);
            return data;
        }

	private:
		mutable std::mutex mutex;

		T data;
	};

}