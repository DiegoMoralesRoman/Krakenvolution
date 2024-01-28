add_subdirectory(easylogging)
add_subdirectory(dbscan)

set(THIRD_PARTY
	rxcpp
	dbscan
	easylogging++
	mini-yaml
)

message("${Yellow}Using the following third party libraries:${Reset}")
foreach(NAME ${THIRD_PARTY})
	message("	- ${NAME}")
endforeach()
message("")

target_compile_options(easylogging PRIVATE -w)
target_compile_options(dbscan PRIVATE -w)
