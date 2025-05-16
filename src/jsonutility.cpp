#include <le-cpp-utils/jsonutility.h>
#include <le-cpp-utils/logging.h>
#include <fstream>
#include <filesystem>

using json = nlohmann::json;

std::optional<json> loadJsonFromFile(const std::string& path)
{
	if (!std::filesystem::exists(path))
		return std::nullopt;

	std::ifstream file(path);
	if (!file.is_open())
	{
		LOG_ERROR("Failed to open JSON file ({}).", path);
		return false;
	}

	try
	{
		json j;
		file >> j;
		return j;
	}
	catch (...)
	{
		LOG_ERROR("Parsing JSON file ({}) failed.", path);	
	}
	return std::nullopt;
}

std::optional<json> loadOrCreateJson(const std::string& path, std::function<json()> defaultJsonGenerator, bool prettySave)
{
	if (std::filesystem::exists(path))
	{
		auto j = loadJsonFromFile(path);
		if (j.has_value())
			return j;
	}
	else
	{
		auto j = defaultJsonGenerator();
		if (saveJsonToFile(path, j, prettySave))
			return j;
	}
	return std::nullopt;
}

bool saveJsonToFile(const std::string& path, const nlohmann::json& j, bool pretty)
{
	std::ofstream of(path);
	if (!of.is_open())
	{
		LOG_ERROR("Failed to open JSON file for writing ({}).", path);
		return false;
	}
	if (pretty)
		of << std::setw(4) << j << std::endl;
	else
		of << j << std::endl;
	return true;
}
