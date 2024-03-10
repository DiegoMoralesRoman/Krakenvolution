file(GLOB_RECURSE FILES
	"${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

list(LENGTH FILES NUM_FILES)
message("${Cyan}${Bold}Found ${NUM_FILES} files for system nodes${Reset}")

add_library(user_nodes ${FILES})
