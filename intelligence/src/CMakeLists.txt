set(MAIN_SOURCES 
	libpubsub
)

add_subdirectory(parser)
add_subdirectory(flag)

# Find SFML
find_package(SFML 2.5 COMPONENTS graphics QUIET)

add_executable(${PROJECT_NAME} main.cpp)
set(MAIN_LIBS
	# Third party
	easylogging

	# src folder
	cliparser
	flag

	# lib folder
	nodes
)

if (SFML_FOUND)
	list(APPEND MAIN_LIBS 
		sfml-graphics
		graphics # Custom one
	)

	message("${Green}Found SFML in system, building with HAS_SFML #define${Reset}")
	target_compile_definitions(${PROJECT_NAME} PRIVATE HAS_SFML)
	add_subdirectory(graphics)
else()
	message("${Yellow}SFML was not found, compiling without graphicsl options${Reset}")
endif()

target_link_libraries(${PROJECT_NAME} 
	${MAIN_LIBS}
)

add_executable(tests tests.cpp)
target_link_libraries(
	${PROJECT_NAME}
)
