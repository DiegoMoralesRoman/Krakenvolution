message("${Green}Compiling graphics lib${Reset}")

file (GLOB_RECURSE SOURCES 
	${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/*.hpp
)

add_library(graphics ${SOURCES})
