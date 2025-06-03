#pragma once

#include <le-cpp-utils/pimpl.h>
#include <functional>
#include <chrono>
#include <mutex>

namespace le
{
	class MtAutoConfiguration
	{
		DECLARE_PIMPL;
	public:
		explicit MtAutoConfiguration(
			std::function<bool()> fnConfigure,
			std::function<void()> fnCleanup
		);
		virtual ~MtAutoConfiguration();

		MtAutoConfiguration(const MtAutoConfiguration&) = delete;
		MtAutoConfiguration& operator=(const MtAutoConfiguration&) = delete;

		void setCbStateChanged(std::function<void(bool)> cbStateChanged);
		void setReconfigurationDelay(std::chrono::milliseconds reconfigurationDelay);
		void setFnHeartbeat(std::function<bool(bool onConfigure)> fnHeartbeat);
		void setHeartbeatTimeout(std::chrono::milliseconds heartbeatTimeout);

		/// <summary>
		/// Starts the auto configuration thread.
		/// </summary>
		/// <returns>True if succesful.</returns>
		void start();

		/// <summary>
		/// Stops the thread and cleans up the configurable object.
		/// </summary>
		void stop();

		/// <summary>
		/// IMPORTANT: The resource lock must be acquired when calling this function.
		/// </summary>
		/// <returns>True if the resource has been configured successfully.</returns>
		bool ok() const { return m_ok; }

		/// <summary>
		/// Indicate that reconfiguration is needed.
		/// IMPORTANT: The resource lock must be acquired when calling this function.
		/// </summary>
		void requestReconfiguration();

		/// <summary>
		/// Users of the configurable resource must acquire the resource lock before using the resource.
		/// This ensures that the automatic configuration thread is not, e.g., sending heartbeats or reconfiguring at the same time.
		/// </summary>
		/// <returns>Lock (locked)</returns>
		std::unique_lock<std::mutex> acquireResourceLock();

	private:
		void autoConfigurationLoop();
		void tryConfigure();
		void setState(bool ok);

		bool m_ok = false;

		std::function<bool()> m_fnConfigure;
		std::function<void()> m_fnCleanup;
		std::function<void(bool)> m_cbStateChanged = nullptr;
		std::chrono::milliseconds m_reconfigurationDelay = std::chrono::milliseconds(5000);

		std::function<bool(bool onConfigure)> m_fnHeartbeat = nullptr;
		std::chrono::milliseconds m_heartbeatTimeout = std::chrono::milliseconds(5000);

	};
}