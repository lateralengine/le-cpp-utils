#pragma once

#include <string>
#include <sstream>
#include <array>

namespace le
{
    std::array<int, 4> Parse(const std::string& input)
    {
        std::array<int, 4> version;
        std::istringstream parser(input);
        parser >> version[0];
        for (int idx = 1; idx < 4; idx++)
        {
            parser.get(); //Skip period
            parser >> version[idx];
        }
        return version;
    }

    bool LessThanVersion(const std::string& a, const std::string& b)
    {
        auto parsedA = Parse(a);
        auto parsedB = Parse(b);
        return std::lexicographical_compare(parsedA.data(), parsedA.data() + 4, parsedB.data(), parsedB.data() + 4);
    }
}