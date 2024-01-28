# Find Yaml library
find_package(yaml-cpp REQUIRED)
include_directories(${YAML_INCLUDE_DIRS})

target_link_libraries(${MOD_NAME} ${YAML_CPP_LIBRARIES})
