set(MAIN_SOURCES 
	libpubsub
)

# Find SFML
find_package(SFML 2.5 COMPONENTS graphics QUIET)

add_subdirectory(run)
add_executable(${PROJECT_NAME} main.cpp ${MAIN_FILES})
set(MAIN_LIBS
	# Third party
	easylogging

	# src folder
	cliparser
	flag

	# Nodes
	user_nodes

	# messages
	messages

	# lib folder
	nodes
	config
	topics
)

if (SFML_FOUND)
	list(APPEND MAIN_LIBS 
		sfml-graphics
		graphics # Custom one
	)

	message("${Green}Found SFML in system, building with HAS_SFML #define${Reset}")
	target_compile_definitions(${PROJECT_NAME} PRIVATE HAS_SFML)
	add_subdirectory(run/graphics)
else()
	message("${Yellow}SFML was not found, compiling without graphical options${Reset}")
endif()

target_link_libraries(${PROJECT_NAME} 
	${MAIN_LIBS}
)

add_executable(tests tests.cpp)
target_link_libraries(tests
	${MAIN_LIBS}
	ftxui
)

add_subdirectory(sender)
add_executable(msgsender sender.cpp ${SENDER_FILES})
target_link_libraries(msgsender
	# Third party
	easylogging
	ftxui
	
	messages
	networking
)

add_subdirectory(sequencer)
add_executable(msgsequencer sequencer.cpp ${SEQUENCER_FILES})
target_link_libraries(msgsequencer
	networking
	easylogging
	messages
)

add_dependencies(tests messages)
add_dependencies(${PROJECT_NAME} messages)
