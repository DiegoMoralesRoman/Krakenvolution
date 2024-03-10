file(GLOB_RECURSE FILES
	"${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

list(LENGTH FILES NUM_FILES)
message("${Green}Found ${NUM_FILES} extra source files for sequencer executable${Reset}")

set(SEQUENCER_FILES "${FILES}" PARENT_SCOPE)
