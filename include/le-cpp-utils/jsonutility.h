#pragma once

#include <le-cpp-utils/time.h>
#include <optional>
#include <nlohmann/json.hpp>

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


std::optional<nlohmann::json> loadJsonFromFile(const std::string& path);

/// <summary>
/// Load JSON from file or create it from defaultJson if file does not exist.
/// </summary>
std::optional<nlohmann::json> loadOrCreateJson(const std::string& path, std::function<nlohmann::json()> defaultJsonGenerator, bool prettySave = true);

bool saveJsonToFile(const std::string& path, const nlohmann::json& j, bool pretty);