cmake_minimum_required(VERSION 3.16)

project(NSGAII_Project LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
endif()

option(BUILD_TESTS "Build unit tests" ON)

include_directories(${CMAKE_SOURCE_DIR}/include)

file(GLOB_RECURSE SRC_FILES ${CMAKE_SOURCE_DIR}/src/*.cpp)

add_library(nsgaii_lib ${SRC_FILES})

add_executable(nsgaii src/main.cpp)
target_link_libraries(nsgaii PRIVATE nsgaii_lib)

if (BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
