# cpp/tests/CMakeLists.txt

# 包含目录
include_directories(${CMAKE_SOURCE_DIR}/include)

# 查找所有测试文件
file(GLOB TEST_FILES ${CMAKE_SOURCE_DIR}/tests/*.cpp)

# 为每个测试文件创建可执行目标
foreach(TEST_FILE ${TEST_FILES})
    # 获取测试文件的名字
    get_filename_component(TEST_NAME ${TEST_FILE} NAME_WE)

    # 添加可执行文件
    add_executable(${TEST_NAME} ${TEST_FILE})

    # 链接到静态库
    target_link_libraries(${TEST_NAME} PRIVATE nsgaii_lib)
endforeach()
