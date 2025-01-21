# cpp/CMakeLists.txt

# Config
cmake_minimum_required(VERSION 3.16)

project(NSGAII_Project LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_BUILD_TYPE Debug)

# Includes
include_directories(${CMAKE_SOURCE_DIR}/include)

# Source
file(GLOB_RECURSE SRC_FILES ${CMAKE_SOURCE_DIR}/src/*.cpp)

# Create a static lib
add_library(nsgaii_lib ${SRC_FILES})

# Link
add_executable(nsgaii src/main.cpp)
target_link_libraries(nsgaii PRIVATE nsgaii_lib)

# Compile test sessions
add_subdirectory(tests)
