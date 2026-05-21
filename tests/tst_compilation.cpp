#include <le-cpp-utils/logging.h>
#include <le-cpp-utils/signal.h>
#include <le-cpp-utils/pimpl.h>
#include <le-cpp-utils/subcommand.h>
#include <le-cpp-utils/debugmutex.h>
#include <le-cpp-utils/endianness.h>
#include <le-cpp-utils/fileread.h>
#include <le-cpp-utils/speedmonitor.h>
#include <le-cpp-utils/persistentvalue.h>
#ifdef _WIN32
#include <le-cpp-utils/winutils.h>
#endif
#include <catch2/catch_test_macros.hpp>
#include <chrono>

class MinimalSubcommand : public le::Subcommand
{
public:
    MinimalSubcommand(CLI::App& app) : Subcommand(app, "minimal", "test subcommand") {}
protected:
    int execute() override { return 0; }
};

class PimplExample
{
public:
    PimplExample();
    ~PimplExample();
    DECLARE_PIMPL;
};

struct PimplExample::Impl {};
PimplExample::PimplExample() : d_ptr(nullptr) {}
PimplExample::~PimplExample() = default;

TEST_CASE("headers compile")
{
    LOG_DEBUG("compilation test");

    CLI::App app;
    MinimalSubcommand sub(app);
    REQUIRE(sub.process() == 0);

    PimplExample pimplObj;

#ifndef NDEBUG
    le::DebugMutex dbgMtx;
    dbgMtx.lock();
    REQUIRE(dbgMtx.isLockedByCurrentThread());
    dbgMtx.unlock();
#endif

    le::readFileContents("nonexistent_le_test.txt");

    {
        le::SpeedMonitor<int> monitor(std::chrono::milliseconds(50), 0.5, 10.0, [] {}, [] {});
    }

    SUCCEED();
}
