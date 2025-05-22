#pragma once

#include <le-cpp-utils/logging.h>
#include <fstream>
#include <optional>
#include <filesystem>

namespace le
{
	template<typename T, bool IsBinary = false>
	class ValueInFile
	{
	public:

		static std::optional<T> load(const std::filesystem::path& path)
		{
			constexpr std::ios_base::openmode openmode = IsBinary ? (std::ios::in | std::ios::binary) : std::ios::in;
			std::ifstream infile(path, openmode);
			if (!infile.is_open())
				return std::nullopt;

			T value;
			infile >> value;
			if (infile.fail())
			{
				LOG_ERROR("Failed to read value from file ({}).", path.string());
				return std::nullopt;
			}
			return std::make_optional(value);
		}

		static bool write(const std::filesystem::path& path, const T& value)
		{
			constexpr std::ios_base::openmode openmode = IsBinary ? (std::ios::out | std::ios::binary) : std::ios::out;
			std::ofstream outfile(path, openmode);
			if (!outfile.is_open())
			{
				LOG_ERROR("Unable to open file for writing ({}).", path.string());
				return false;
			}
			outfile << value << std::flush;
			if (outfile.fail())
			{
				LOG_ERROR("Failed to write data to file ({}).", path.string());
				return false;
			}
			return true;
		}
	};
}