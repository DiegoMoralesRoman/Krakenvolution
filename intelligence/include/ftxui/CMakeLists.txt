file(GLOB_RECURSE HEADERS
	${CMAKE_CURRENT_SOURCE_DIR}/*.hpp
)

set(COMPILED
	${CMAKE_CURRENT_SOURCE_DIR}/lib/libftxui-component.a
	${CMAKE_CURRENT_SOURCE_DIR}/lib/libftxui-dom.a
	${CMAKE_CURRENT_SOURCE_DIR}/lib/libftxui-screen.a
)

add_library(ftxui ${HEADERS})
target_link_libraries(ftxui ${COMPILED})
set_target_properties(ftxui PROPERTIES LINKER_LANGUAGE CXX)
