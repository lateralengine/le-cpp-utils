#include <le-cpp-utils/jsonutility.h>
#include <le-cpp-utils/finally.h>
#include <catch2/catch_test_macros.hpp>
#include <filesystem>

struct TestConfig
{
    int value = 0;
    std::string name = "default";
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(TestConfig, value, name)

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
