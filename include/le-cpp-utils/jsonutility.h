#pragma once

#include <le-cpp-utils/logging.h>
#include <le-cpp-utils/time.h>
#include <optional>
#include <functional>
#include <nlohmann/json.hpp>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <string>

namespace nlohmann
{

	template<>
	struct adl_serializer<std::chrono::system_clock::time_point>
	{
		static void to_json(json& j, const std::chrono::system_clock::time_point& value)
		{
			j = le::timeIso8601Utc(value);
		}
	};

	template<typename T>
	struct adl_serializer<std::optional<T>>
	{
		static void from_json(const json& j, std::optional<T>& value)
		{
			if (j.is_null())
				value = std::nullopt;
			else
				value = j.get<T>();
		}
		static void to_json(json& j, const std::optional<T>& t)
		{
			if (t)
				adl_serializer<T>::to_json(j, *t);
			else
				j = nullptr;
		}
	};
}

#define DEFINE_TO_JSON_NON_INTRUSIVE(Type, ...)  \
    inline void to_json(nlohmann::json& nlohmann_json_j, const Type& nlohmann_json_t) { NLOHMANN_JSON_EXPAND(NLOHMANN_JSON_PASTE(NLOHMANN_JSON_TO, __VA_ARGS__)) } \

#define DEFINE_FROM_JSON_NON_INTRUSIVE(Type, ...)  \
    inline void from_json(const nlohmann::json& nlohmann_json_j, Type& nlohmann_json_t) { NLOHMANN_JSON_EXPAND(NLOHMANN_JSON_PASTE(NLOHMANN_JSON_FROM, __VA_ARGS__)) }


namespace le {

inline std::optional<nlohmann::json> loadJsonFromFile(const std::string& path)
{
	if (!std::filesystem::exists(path))
		return std::nullopt;

	std::ifstream file(path);
	if (!file.is_open())
	{
		LOG_ERROR("Failed to open JSON file ({}).", path);
		return std::nullopt;
	}

	try
	{
		nlohmann::json j;
		file >> j;
		return j;
	}
	catch (...)
	{
		LOG_ERROR("Parsing JSON file ({}) failed.", path);
	}
	return std::nullopt;
}

inline bool saveJsonToFile(const std::string& path, const nlohmann::json& j, bool pretty = true)
{
	std::ofstream file(path, std::ios::out | std::ios::trunc);
	if (!file.is_open())
	{
		LOG_ERROR("Failed to open JSON file for writing ({}).", path);
		return false;
	}
	if (pretty)
		file << std::setw(4) << j << std::endl;
	else
		file << j << std::endl;
	return file.good();
}

inline std::optional<nlohmann::json> loadOrCreateJson(
	const std::string& path,
	std::function<nlohmann::json()> defaultJsonGenerator,
	bool prettySave = true)
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

template<typename T>
class JsonLoader
{
public:

	static bool load(const std::string& path, T& out, bool createIfMissing = true)
	{
		auto j = loadJsonFromFile(path);
		if (!j)
		{
			if (!createIfMissing)
			{
				LOG_ERROR("JSON file ({}) not found.", path);
				return false;
			}
			LOG_WARN("JSON file ({}) not found. Creating with default values.", path);
			out = T{};
			return save(out, path);
		}
		try
		{
			out = j->template get<T>();
			LOG_DEBUG("JSON file loaded ({}).", path);
			return true;
		}
		catch (const nlohmann::json::exception& e)
		{
			LOG_ERROR("Parsing JSON failed ({}).", e.what());
			return false;
		}
		catch (...)
		{
			LOG_ERROR("Parsing JSON failed.");
			return false;
		}
	}

	static bool save(const T& value, const std::string& path, bool prettyJson = true)
	{
		return saveJsonToFile(path, nlohmann::json(value), prettyJson);
	}
};

} // namespace le
