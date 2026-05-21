include(FetchContent)

FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG        v3.15.0
    FIND_PACKAGE_ARGS CONFIG
)

FetchContent_MakeAvailable(Catch2)

add_executable(le-cpp-utils-tests
    tst_consumer.cpp
    tst_median.cpp
    tst_mtautoconfiguration.cpp
    tst_threadsafequeue.cpp
    tst_finally.cpp
    tst_versioncompare.cpp
    tst_utilities.cpp
    tst_threadsafe.cpp
    tst_uuid.cpp
    tst_valueinfile.cpp
    tst_jsonutility.cpp
    tst_compilation.cpp
)

target_link_libraries(le-cpp-utils-tests
    PRIVATE
        le-cpp-utils::le-cpp-utils
        Catch2::Catch2WithMain
)

include(Catch)
catch_discover_tests(le-cpp-utils-tests)