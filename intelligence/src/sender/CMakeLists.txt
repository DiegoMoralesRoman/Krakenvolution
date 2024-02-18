file(GLOB_RECURSE FILES
	"${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

list(LENGTH FILES NUM_FILES)
message("${Green}Found ${NUM_FILES} extra source files for sender executable${Reset}")

set(SENDER_FILES "${FILES}" PARENT_SCOPE)

add_subdirectory(parser)
