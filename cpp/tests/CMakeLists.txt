# cpp/tests/CMakeLists.txt

include_directories(${CMAKE_SOURCE_DIR}/include)

file(GLOB TEST_FILES ${CMAKE_SOURCE_DIR}/tests/*.cpp)

foreach(TEST_FILE ${TEST_FILES})
    get_filename_component(TEST_NAME ${TEST_FILE} NAME_WE)
    add_executable(${TEST_NAME} ${TEST_FILE})
    target_link_libraries(${TEST_NAME} PRIVATE nsgaii_lib)
    
endforeach()
