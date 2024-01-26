file(GLOB_RECURSE PROTO_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/*.proto
)

find_package(Protobuf REQUIRED)
find_package(absl REQUIRED)
include_directories(${PROTOBUF_INCLUDE_DIR})

message("${Green}Found Protobuf sources: ${Reset}")
foreach(SOURCE ${PROTO_FILES})
	message("	- ${SOURCE}")
endforeach()
message("")

set(PROTO_OUTPUT_DIR "${CMAKE_SOURCE_DIR}/messages/build")
set(PROTO_OUTPUT_DIR "${PROTO_OUTPUT_DIR}" PARENT_SCOPE)

set(PROTOBUF_GENERATE_CPP_APPEND_PATH FALSE)

# Generate C++ sources and headers from proto files
file(MAKE_DIRECTORY "${PROTO_OUTPUT_DIR}")
file(MAKE_DIRECTORY "${PROTO_OUTPUT_DIR}/messages")
set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "${PROTO_OUTPUT_DIR}")

protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${PROTO_FILES}
					 PROTOC_OUT_DIR "${PROTO_OUTPUT_DIR}/messages")

add_library(messages
	${PROTO_SRCS}
	${PROTO_HDRS}
)

target_link_libraries(messages PUBLIC
	protobuf::libprotobuf
	absl::strings 
	absl::base
	absl::log_internal_check_op
)

target_include_directories(messages PUBLIC ${PROTO_OUTPUT_DIR})
