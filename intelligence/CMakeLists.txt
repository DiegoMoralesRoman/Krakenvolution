cmake_minimum_required(VERSION 3.10)

# Define colors for CMake messages
if(NOT WIN32)
  string(ASCII 27 Esc)
  set(Reset "${Esc}[m")
  set(Bold  "${Esc}[1m")
  set(Red         "${Esc}[31m")
  set(Green       "${Esc}[32m")
  set(Yellow      "${Esc}[33m")
  set(Blue        "${Esc}[34m")
  set(Magenta     "${Esc}[35m")
  set(Cyan        "${Esc}[36m")
  set(White       "${Esc}[37m")
  set(BoldRed     "${Esc}[1;31m")
  set(BoldGreen   "${Esc}[1;32m")
  set(BoldYellow  "${Esc}[1;33m")
  set(BoldBlue    "${Esc}[1;34m")
  set(BoldMagenta "${Esc}[1;35m")
  set(BoldCyan    "${Esc}[1;36m")
  set(BoldWhite   "${Esc}[1;37m")
endif()

project(krakenvolution)
set(CMAKE_CXX_STANDARD 23)

set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")

# For RXCPP reasons I have to disable this warnings
add_compile_options(-Wno-deprecated-declarations)

add_subdirectory(messages)
include_directories(${PROTO_OUTPUT_DIR})

include_directories(${PROJECT_SOURCE_DIR}/lib)
include_directories(${PROJECT_SOURCE_DIR}/include)

add_subdirectory(include)
add_subdirectory(src)
add_subdirectory(lib)


add_custom_target(run
	COMMAND ${CMAKE_COMMAND} --build . --target ${PROJECT_NAME}
	COMMAND ./src/${PROJECT_NAME}
	WORKING_DIRECTORY ${CMAKE_PROJECT_DIR}
	COMMENT "Builds and runs the entire application"
)
add_dependencies(run ${PROJECT_NAME})

add_custom_target(valg
	COMMAND ${CMAKE_COMMAND} --build . --target ${PROJECT_NAME}
	COMMAND valgrind --leak-check=full ./src/${PROJECT_NAME}
	WORKING_DIRECTORY ${CMAKE_PROJECT_DIR}
	COMMENT "Builds and runs the entire application with valgrind"
)
add_dependencies(valg ${PROJECT_NAME})

enable_testing()
add_subdirectory(test)

set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")

add_custom_target(uninstall
    COMMAND "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake"
)

install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib/static)
