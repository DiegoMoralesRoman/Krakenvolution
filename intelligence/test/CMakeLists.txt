find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

function(add_gtest TEST_NAME TEST_FILE)
	file(RELATIVE_PATH REL_PATH ${CMAKE_CURRENT_SOURCE_DIR} ${TEST_FILE})
	message("	- ${REL_PATH} -> ${TEST_NAME}")

	add_executable(${TEST_NAME} ${TEST_FILE})
	target_link_libraries(${TEST_NAME} ${GTEST_BOTH_LIBRARIES} ${ALL_LIBS})
	add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endfunction()

file(GLOB_RECURSE TEST_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/test_*.cpp
)

if (NOT "${TEST_FILES}")
	message("\n${Green}Found tests:${Reset}")
	foreach(TEST_FILE ${TEST_FILES})
		get_filename_component(TEST_NAME ${TEST_FILE} NAME_WE) # Get the file name without extension
		add_gtest(${TEST_NAME} ${TEST_FILE})
	endforeach()
	message("")
else()
	message("\n${Yellow}No tests were found in test dir\n")
endif()
