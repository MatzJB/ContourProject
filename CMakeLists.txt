#set(CMAKE_FIND_DEBUG_MODE 1)

# Set the minimum CMake version required
cmake_minimum_required(VERSION 3.16)

# Set the project name and version
project(ContourProject VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if (CMAKE_CONFIGURATION_TYPES)
    set(CTEST_CONFIGURATION_TYPE "Debug")
endif()

# Add include directories
include_directories(${CMAKE_SOURCE_DIR}/include)

# Collect source files EXCLUDING main.cpp
file(GLOB_RECURSE LIB_SOURCES
    ${CMAKE_SOURCE_DIR}/src/*.cpp
)

# Remove src/main.cpp from LIB_SOURCES
list(REMOVE_ITEM LIB_SOURCES "${CMAKE_SOURCE_DIR}/src/main.cpp")

# Create the library
add_library(ContourLib ${LIB_SOURCES})

# Add the main executable (the application)
add_executable(ContourProjectMain src/main.cpp)
target_link_libraries(ContourProjectMain PRIVATE ContourLib)
# target_link_libraries(ContourProjectMain PRIVATE gtest gtest_main)

# Set build type
set(CMAKE_BUILD_TYPE Debug)

# Add Google Test using FetchContent
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        6910c9d  # 1.16.0
)

set(BUILD_GMOCK OFF CACHE BOOL "" FORCE) # Disable GMock
FetchContent_MakeAvailable(googletest)
set(CTEST_CONFIGURATION_TYPE Debug)
enable_testing()


#target_link_libraries(ContourTests PRIVATE gtest gtest_main ContourLib)
#add_test(NAME Tests COMMAND ContourTests.exe WORKING_DIRECTORY ${UNIT_TEST_BIN_OUTPUT_DIR})

file(GLOB UNIT_TEST_SOURCES tests/*.cpp)

add_executable(ContourTests ${UNIT_TEST_SOURCES})
target_link_libraries(ContourTests gtest_main ContourLib)

add_test(NAME unit_tests COMMAND unit_tests)