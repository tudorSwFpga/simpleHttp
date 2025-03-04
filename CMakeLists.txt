# Set the CMake minimum version
cmake_minimum_required(VERSION 3.10)

# Set the project name
project(HttpClient)
find_package(spdlog REQUIRED)
if (NOT spdlog_FOUND)
    message(STATUS "spdlog not found, installing through apt")
    execute_process(COMMAND sudo apt install -y libspdlog-dev)
endif()

option(BUILD_TESTS "Enable building the tests" OFF)

# Add the source directory to the build
add_subdirectory(source)

# Add the test directory to the build
add_subdirectory(sample)

if(BUILD_TESTS)
    # Add the test directory to the build
    add_subdirectory(tests)
endif()

