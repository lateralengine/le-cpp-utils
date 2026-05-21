#include <le-cpp-utils/valueinfile.h>
#include <le-cpp-utils/finally.h>
#include <catch2/catch_test_macros.hpp>
#include <filesystem>

TEST_CASE("ValueInFile")
{
    auto tempPath = std::filesystem::temp_directory_path() / "le_test_valueinfile.tmp";
    le::Finally cleanup([&] { std::filesystem::remove(tempPath); });

    SECTION("write and load round-trip")
    {
        REQUIRE(le::ValueInFile<int>::write(tempPath, 1234));
        auto loaded = le::ValueInFile<int>::load(tempPath);
        REQUIRE(loaded.has_value());
        REQUIRE(*loaded == 1234);
    }

    SECTION("load from nonexistent path returns nullopt")
    {
        auto result = le::ValueInFile<int>::load("nonexistent_le_test.tmp");
        REQUIRE_FALSE(result.has_value());
    }
}
