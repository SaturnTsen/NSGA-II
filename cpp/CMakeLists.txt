# cpp/CMakeLists.txt

# 设置最低 CMake 版本
cmake_minimum_required(VERSION 3.16)

# 项目信息
project(NSGAII_Project LANGUAGES CXX)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 启用调试信息（可选）
set(CMAKE_BUILD_TYPE Debug)

# 包含目录
include_directories(${CMAKE_SOURCE_DIR}/include)

# 源文件目录
file(GLOB_RECURSE SRC_FILES ${CMAKE_SOURCE_DIR}/src/*.cpp)

# 创建一个静态库
add_library(nsgaii_lib ${SRC_FILES})

# 添加可执行文件，并链接静态库
add_executable(nsgaii src/main.cpp)
target_link_libraries(nsgaii PRIVATE nsgaii_lib)

# 添加测试子目录
add_subdirectory(tests)
