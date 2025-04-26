# Set the minimum CMake version required
cmake_minimum_required(VERSION 3.21)

# Set the project name and version
project(ContourProject VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add include directories
include_directories(${CMAKE_SOURCE_DIR}/include)

# Collect all source files from src/
file(GLOB_RECURSE SOURCES ${CMAKE_SOURCE_DIR}/src/*.cpp)

# Create a library for shared code
add_library(ContourLib ${SOURCES})

# Add the main executable target
add_executable(ContourProjectMain src/main.cpp)
target_link_libraries(ContourProjectMain ContourLib)

# Optionally, enable debugging symbols
set(Python_EXECUTABLE "path/to/python")
set(CMAKE_BUILD_TYPE Debug)

# Add Google Test using FetchContent
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/release-1.12.1.zip
)
FetchContent_MakeAvailable(googletest)

# Enable testing
enable_testing()

# Add the test executable
add_executable(ContourTests tests/test_main.cpp)
target_link_libraries(ContourTests gtest gtest_main ContourLib)

# Use the paths provided by FetchContent
include_directories(${googletest_SOURCE_DIR}/googletest/include)
include_directories(${googletest_SOURCE_DIR}/googlemock/include)

# Remove find_package(GTest REQUIRED) as it is not needed
# Remove the message line for GTEST_INCLUDE_DIRS

target_link_libraries(ContourTests gtest gtest_main ContourLib)


target_link_libraries(ContourTests gtest gtest_main ContourLib)

add_test(THREADTEST ContourTests COMMAND ContourTests)
