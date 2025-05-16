#pragma once

#include <filesystem>
#include <string_view>

namespace le
{
	std::string readFileContents(const std::filesystem::path& path);
}