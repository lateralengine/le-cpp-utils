#pragma once

#include <time.h>
#include <ctime>
#include <string>

namespace le
{
    inline std::string timeString(std::time_t time, const std::string& fmt = "%FT%T%z")
    {
        std::tm bt{};
        localtime_s(&bt, &time);

        char buf[64];
        return { buf, std::strftime(buf, sizeof(buf), fmt.c_str(), &bt) };
    }
}