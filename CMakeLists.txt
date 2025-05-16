cmake_minimum_required(VERSION 3.16.0)

project(le-cpp-utils VERSION 0.1.0 LANGUAGES CXX)


find_package(nlohmann_json CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(libenvpp CONFIG REQUIRED)

set(LE_CPP_UTILS_HEADERS
    include/le-cpp-utils/CLI11.hpp
    include/le-cpp-utils/consumer.h
    include/le-cpp-utils/debugmutex.h
    include/le-cpp-utils/envconfigloader.h
    include/le-cpp-utils/fileread.h
    include/le-cpp-utils/finally.h
    include/le-cpp-utils/jsonutility.h
    include/le-cpp-utils/logging.h
    include/le-cpp-utils/median.h
    include/le-cpp-utils/mtautoconfiguration.h
    include/le-cpp-utils/persistentvalue.h
    include/le-cpp-utils/pimpl.h
    include/le-cpp-utils/speedmonitor.h
    include/le-cpp-utils/subcommand.h
    include/le-cpp-utils/threadsafedata.h
    include/le-cpp-utils/threadsafeevent.h
    include/le-cpp-utils/threadsafequeue.h
    include/le-cpp-utils/time.h
    include/le-cpp-utils/timestring.h
    include/le-cpp-utils/valueinfile.h
    include/le-cpp-utils/versioncompare.h
    include/le-cpp-utils/winutils.h
)

add_library(le-cpp-utils
    ${LE_CPP_UTILS_HEADERS}
    src/fileread.cpp
    src/jsonutility.cpp
    src/mtautoconfiguration.cpp
    src/winutils.cpp
)

target_compile_features(le-cpp-utils PRIVATE cxx_std_20)

target_include_directories(le-cpp-utils PUBLIC include)
target_link_libraries(le-cpp-utils PUBLIC
    nlohmann_json::nlohmann_json
    libenvpp::libenvpp
    spdlog::spdlog
)

set_target_properties(
    le-cpp-utils PROPERTIES
    PUBLIC_HEADER "${LE_CPP_UTILS_HEADERS}"
)

include(GNUInstallDirs)
install(TARGETS le-cpp-utils
    EXPORT le-cpp-utils_targets
    ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/le-cpp-utils
)