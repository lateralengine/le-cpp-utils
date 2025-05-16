#pragma once

#include <chrono>
#include <string>

namespace le
{
	inline int64_t timeMsSinceEpoch()
	{
		return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
	}

	inline std::string timeIso8601Utc(const std::chrono::system_clock::time_point& time)
	{
		return std::format("{:%FT%TZ}", time);
	}
}