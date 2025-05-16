cmake_minimum_required(VERSION 3.16.0)

project(le-cpp-utils VERSION 0.1.0 LANGUAGES CXX)

find_package(nlohmann_json CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(libenvpp CONFIG REQUIRED)

add_library(le-cpp-utils
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

