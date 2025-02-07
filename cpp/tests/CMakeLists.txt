include_directories(${CMAKE_SOURCE_DIR}/include)

file(GLOB TEST_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

foreach(TEST_FILE ${TEST_FILES})
    get_filename_component(TEST_NAME ${TEST_FILE} NAME_WE)
    add_executable(${TEST_NAME} ${TEST_FILE})
    target_link_libraries(${TEST_NAME} PRIVATE nsgaii_lib)
    
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endforeach()
