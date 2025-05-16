#include <le-cpp-utils/mtautoconfiguration.h>

#include <exception>
#include <thread>

using namespace le;

class MtAutoConfiguration::Impl
{
public:
	Impl() {}

	~Impl() {}

	bool stopRequested() const { return stopFlag; }

	std::thread thread;
	mutable std::mutex mutex;
	std::condition_variable cv;
	bool stopFlag = false;
};

MtAutoConfiguration::MtAutoConfiguration(
	std::function<bool()> fnConfigure,
	std::function<void()> fnCleanup
) :
	d_ptr(new Impl()),
	m_fnConfigure(fnConfigure),
	m_fnCleanup(fnCleanup)
{}

MtAutoConfiguration::~MtAutoConfiguration() {}

void MtAutoConfiguration::setCbStateChanged(std::function<void(bool)> cbStateChanged)
{
	PIMPL;
	if (d->thread.joinable())
		throw std::exception("MtAutoConfiguration already started");
	m_cbStateChanged = cbStateChanged;
}

void MtAutoConfiguration::setReconfigurationDelay(std::chrono::milliseconds reconfigurationDelay)
{
	PIMPL;
	if (d->thread.joinable())
		throw std::exception("MtAutoConfiguration already started");
	m_reconfigurationDelay = reconfigurationDelay;
}

void MtAutoConfiguration::setFnHeartbeat(std::function<bool(bool onConfigure)> fnHeartbeat)
{
	PIMPL;
	if (d->thread.joinable())
		throw std::exception("MtAutoConfiguration already started");
	m_fnHeartbeat = fnHeartbeat;
}

void MtAutoConfiguration::setHeartbeatTimeout(std::chrono::milliseconds heartbeatTimeout)
{
	PIMPL;
	if (d->thread.joinable())
		throw std::exception("MtAutoConfiguration already started");
	m_heartbeatTimeout = heartbeatTimeout;
}

void MtAutoConfiguration::start()
{
	PIMPL;
	if (m_ok || d->thread.joinable())
		throw std::exception("MtAutoConfiguration already started");

	d->stopFlag = false;
	d->thread = std::thread(&MtAutoConfiguration::autoConfigurationLoop, this);
}

void MtAutoConfiguration::stop()
{
	PIMPL;
	if (d->thread.joinable())
	{
		{
			std::scoped_lock lock(d->mutex);
			d->stopFlag = true;
		}
		d->cv.notify_one();
		d->thread.join();
	}
	setState(false);
	m_fnCleanup();
}

void MtAutoConfiguration::requestReconfiguration()
{
	PIMPL;
	setState(false);
	d->cv.notify_one();
}

std::unique_lock<std::mutex> MtAutoConfiguration::acquireResourceLock()
{
	PIMPL;
	return std::unique_lock<std::mutex>(d->mutex);
}

void MtAutoConfiguration::autoConfigurationLoop()
{
	PIMPL;
	while (true)
	{
		std::unique_lock<std::mutex> lk(d->mutex);

		if (!m_ok)
			tryConfigure();

		bool noTimeout = true;
		if (m_ok)
		{
			if (m_fnHeartbeat)
				noTimeout = d->cv.wait_for(
					lk,
					m_heartbeatTimeout,
					[this, d] { return d->stopFlag || !m_ok; }
				);
			else
				d->cv.wait(
					lk,
					[this, d] { return d->stopFlag || !m_ok; }
				);
		}
		else
		{
			noTimeout = d->cv.wait_for(
				lk,
				m_reconfigurationDelay,
				[this, d] { return d->stopFlag; }
			);
		}
		
		if (noTimeout)
		{
			// Either stop flag is true or derived class has set m_ok to false
			if (d->stopRequested())
				return;
		}
		else
		{
			// Heartbeat timeout
			if (m_ok)
				setState(m_fnHeartbeat(false));
		}
	}
}

void MtAutoConfiguration::tryConfigure()
{
	PIMPL;
	setState(false);
	m_fnCleanup();
	if (m_fnConfigure())
	{
		setState(!m_fnHeartbeat || m_fnHeartbeat(true));
	}
}

void MtAutoConfiguration::setState(bool ok)
{
	PIMPL;
	if (m_ok != ok)
	{
		m_ok = ok;
		if (m_cbStateChanged)
			m_cbStateChanged(m_ok);
	}
}
