cmake_minimum_required(VERSION 3.16.0)

set(le-cpp-utils_VERSION 0.1.5)

project(le-cpp-utils VERSION ${le-cpp-utils_VERSION} LANGUAGES CXX)

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

find_package(nlohmann_json CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)

set(LE_CPP_UTILS_HEADERS
    include/le-cpp-utils/CLI11.hpp
    include/le-cpp-utils/consumer.h
    include/le-cpp-utils/debugmutex.h
    include/le-cpp-utils/fileread.h
    include/le-cpp-utils/finally.h
    include/le-cpp-utils/jsonutility.h
    include/le-cpp-utils/logging.h
    include/le-cpp-utils/median.h
    include/le-cpp-utils/mtautoconfiguration.h
    include/le-cpp-utils/persistentvalue.h
    include/le-cpp-utils/pimpl.h
    include/le-cpp-utils/speedmonitor.h
    include/le-cpp-utils/signal.h
    include/le-cpp-utils/subcommand.h
    include/le-cpp-utils/threadinterval.h
    include/le-cpp-utils/threadsafedata.h
    include/le-cpp-utils/threadsafeevent.h
    include/le-cpp-utils/threadsafequeue.h
    include/le-cpp-utils/time.h
    include/le-cpp-utils/valueinfile.h
    include/le-cpp-utils/versioncompare.h
    include/le-cpp-utils/winutils.h
    include/le-cpp-utils/uuid_v4.h
    include/le-cpp-utils/endianness.h
)

add_library(le-cpp-utils
    ${LE_CPP_UTILS_HEADERS}
    src/fileread.cpp
    src/jsonutility.cpp
    src/mtautoconfiguration.cpp
    src/winutils.cpp
    src/signal.cpp
)

target_compile_features(le-cpp-utils PRIVATE cxx_std_20)

target_include_directories(le-cpp-utils
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(le-cpp-utils PUBLIC
    nlohmann_json::nlohmann_json
    spdlog::spdlog
)

configure_package_config_file(cmake/config.cmake.in
	${CMAKE_CURRENT_BINARY_DIR}/le-cpp-utils-config.cmake
	INSTALL_DESTINATION ${CMAKE_INSTALL_DATADIR}/le-cpp-utils
	NO_SET_AND_CHECK_MACRO
)
write_basic_package_version_file(
	${CMAKE_CURRENT_BINARY_DIR}/le-cpp-utils-config-version.cmake
	VERSION ${le-cpp-utils_VERSION}
	COMPATIBILITY ExactVersion
)

install(
    FILES
        ${CMAKE_CURRENT_BINARY_DIR}/le-cpp-utils-config.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/le-cpp-utils-config-version.cmake
    DESTINATION
        ${CMAKE_INSTALL_DATADIR}/le-cpp-utils
)
install(TARGETS le-cpp-utils EXPORT le-cpp-utils-targets)
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(EXPORT le-cpp-utils-targets NAMESPACE le-cpp-utils:: DESTINATION ${CMAKE_INSTALL_DATADIR}/le-cpp-utils)