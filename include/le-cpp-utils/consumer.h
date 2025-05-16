#pragma once

#include <le-cpp-utils/logging.h>
#include <le-cpp-utils/threadsafequeue.h>
#include <memory>

namespace le
{

	template <typename Worker, typename DataIn, typename DataOut = void>
	class Consumer
	{

	public:
		explicit Consumer(std::unique_ptr<Worker> worker, std::function<void(DataOut)> cbDataOut = {}) : m_worker( std::move(worker) ), m_cbDataOut(cbDataOut) {}
		~Consumer() { stop(); }

		void push(DataIn data)
		{
			{
				std::scoped_lock lock(m_mutex);
				m_queue.push(data);
			}
			m_cv.notify_one();
		}

		bool configure()
		{
			if (!m_worker->configure())
				return false;
			start();
			return true;
		}

		void cleanup()
		{
			stop();
			m_worker->cleanup();
		}

	private:
		void start()
		{
			if (m_thread.joinable())
				throw std::logic_error("Consumer thread already started!");
			m_thread = std::thread(&Consumer::loop, this);
		}

		void stop()
		{
			if (m_thread.joinable())
			{
				{
					std::scoped_lock lock(m_mutex);
					m_stopFlag = true;
				}
				m_cv.notify_one();
				m_thread.join();
			}
		}

		void loop()
		{
			while (true)
			{
				std::unique_lock<std::mutex> lock(m_mutex);
				m_cv.wait(lock, [this] {return !m_queue.empty() || m_stopFlag ;});
				if (m_stopFlag)
				{
					if (!m_queue.empty())
					{
						LOG_WARN("Consumer was stopped when were still items in the queue.");
						m_queue = std::queue<DataIn>();
					}
					return;
				}

				DataIn data = m_queue.front();
				m_queue.pop();

				lock.unlock();

				process(data);
			}
		}

		template<typename E = DataOut>
		typename std::enable_if<!std::is_same<E, void>::value, void>::type process(DataIn data)
		{
			DataOut dataOut = m_worker->process(data);
			if (m_cbDataOut)
				m_cbDataOut(dataOut);
		}

		template<typename E = DataOut>
		typename std::enable_if<std::is_same<E, void>::value, void>::type process(DataIn data)
		{
			m_worker->process(data);
			if (m_cbDataOut)
				m_cbDataOut();
		}

		std::unique_ptr<Worker> m_worker;

		std::thread m_thread;
		std::queue<DataIn> m_queue;
		mutable std::mutex m_mutex;
		std::condition_variable m_cv;
		bool m_stopFlag = false;
		
		std::function<void(DataOut)> m_cbDataOut;

	};
}

