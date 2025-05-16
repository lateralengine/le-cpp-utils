#pragma once
#include <spdlog/spdlog.h>

#define LOG_DEBUG(msg, ...) spdlog::debug("{}: " msg, __FUNCTION__, ##__VA_ARGS__)
#define LOG_INFO(msg, ...) spdlog::info("{}: " msg, __FUNCTION__, ##__VA_ARGS__)
#define LOG_WARN(msg, ...) spdlog::warn("{}: " msg, __FUNCTION__, ##__VA_ARGS__)
#define LOG_ERROR(msg, ...) spdlog::error("{}: " msg, __FUNCTION__, ##__VA_ARGS__)
#define LOG_CRITICAL(msg, ...) spdlog::critical("{}: " msg, __FUNCTION__, ##__VA_ARGS__)
#define IS_DEBUG_LEVEL spdlog::should_log(spdlog::level::debug)

namespace le
{
	/// <summary>
	/// This snippet can be included in a compilation unit to allow plugging into the application logging framework.
	/// </summary>
	inline std::shared_ptr<spdlog::logger> setupLogger(std::vector<spdlog::sink_ptr> sinks, std::string name = "")
	{
		auto logger = std::make_shared<spdlog::logger>(name, std::begin(sinks), std::end(sinks));
		spdlog::set_default_logger(logger);
		return logger;
	}
}