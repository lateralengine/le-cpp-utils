#include <le-cpp-utils/fileread.h>

#include <fstream>

std::string le::readFileContents(const std::filesystem::path& path)
{
	if (!std::filesystem::exists(path))
		return {};

	std::ifstream file(path);
	if (!file.is_open())
		return {};
	return std::string((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));
}
