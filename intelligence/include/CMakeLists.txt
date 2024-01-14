add_subdirectory(easylogging)
add_subdirectory(dbscan)

target_compile_options(easylogging PRIVATE -w)
target_compile_options(dbscan PRIVATE -w)
