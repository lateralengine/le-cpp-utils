#include <le-cpp-utils/jsonutility.h>
#include <le-cpp-utils/finally.h>
#include <catch2/catch_test_macros.hpp>
#include <filesystem>
#include <string>

struct TestConfig
{
    int value = 0;
    std::string name = "default";
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(TestConfig, value, name)

TEST_CASE("loadJsonFromFile / saveJsonToFile")
{
    auto tempPath = (std::filesystem::temp_directory_path() / "le_test_raw_json.tmp").string();
    le::Finally cleanup([&] { std::filesystem::remove(tempPath); });

    SECTION("save and load round-trip")
    {
        nlohmann::json original = {{"key", "value"}, {"num", 42}};
        REQUIRE(le::saveJsonToFile(tempPath, original));

        auto loaded = le::loadJsonFromFile(tempPath);
        REQUIRE(loaded.has_value());
        REQUIRE(loaded->at("key") == "value");
        REQUIRE(loaded->at("num") == 42);
    }

    SECTION("load missing file returns nullopt")
    {
        auto result = le::loadJsonFromFile("nonexistent_le_test_raw.json");
        REQUIRE_FALSE(result.has_value());
    }
}

TEST_CASE("loadOrCreateJson")
{
    auto tempPath = (std::filesystem::temp_directory_path() / "le_test_create_json.tmp").string();
    le::Finally cleanup([&] { std::filesystem::remove(tempPath); });

    SECTION("creates file when missing")
    {
        auto result = le::loadOrCreateJson(tempPath, [] {
            return nlohmann::json{{"created", true}};
        });
        REQUIRE(result.has_value());
        REQUIRE(result->at("created") == true);
        REQUIRE(std::filesystem::exists(tempPath));
    }

    SECTION("loads existing file without calling generator")
    {
        nlohmann::json existing = {{"existing", 99}};
        REQUIRE(le::saveJsonToFile(tempPath, existing));

        bool generatorCalled = false;
        auto result = le::loadOrCreateJson(tempPath, [&] {
            generatorCalled = true;
            return nlohmann::json{};
        });
        REQUIRE(result.has_value());
        REQUIRE(result->at("existing") == 99);
        REQUIRE_FALSE(generatorCalled);
    }
}

TEST_CASE("JsonLoader")
{
    auto tempPath = (std::filesystem::temp_directory_path() / "le_test_json.tmp").string();
    le::Finally cleanup([&] { std::filesystem::remove(tempPath); });

    SECTION("save and load round-trip")
    {
        TestConfig original{42, "hello"};
        REQUIRE(le::JsonLoader<TestConfig>::save(original, tempPath));

        TestConfig loaded;
        REQUIRE(le::JsonLoader<TestConfig>::load(tempPath, loaded, false));
        REQUIRE(loaded.value == 42);
        REQUIRE(loaded.name == "hello");
    }

    SECTION("load missing file with createIfMissing=false returns false")
    {
        TestConfig out;
        REQUIRE_FALSE(le::JsonLoader<TestConfig>::load("nonexistent_le_test.json", out, false));
    }

    SECTION("load missing file with createIfMissing=true creates file with defaults")
    {
        auto path = (std::filesystem::temp_directory_path() / "le_test_json_create.tmp").string();
        le::Finally cleanup2([&] { std::filesystem::remove(path); });

        TestConfig out;
        REQUIRE(le::JsonLoader<TestConfig>::load(path, out, true));
        REQUIRE(std::filesystem::exists(path));
        REQUIRE(out.value == 0);
        REQUIRE(out.name == "default");
    }
}
