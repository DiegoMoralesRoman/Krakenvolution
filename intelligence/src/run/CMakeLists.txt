file(GLOB FILES
	"${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

list(LENGTH FILES NUM_FILES)
message("${Green}Found ${NUM_FILES} extra source files for main executable${Reset}")

set(MAIN_FILES "${FILES}" PARENT_SCOPE)

add_subdirectory(flag)
add_subdirectory(parser)
