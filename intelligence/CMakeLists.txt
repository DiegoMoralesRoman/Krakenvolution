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

set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

if (CMAKE_EXPORT_COMPILE_COMMANDS)
	set(CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES
		${CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES})
endif()

project(krakenvolution)
set(CMAKE_CXX_STANDARD 23)

set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "-O3 -g")

# For RXCPP reasons I have to disable this warnings
add_compile_options(-Wno-deprecated-declarations)

# set(CMAKE_CXX_INCLUDE_WHAT_YOU_USE include-what-you-use)
# set(CLANG_TIDY_CHECKS "-checks=-*,modernize-*,llvm-include-order")
# set(CLANG_TIDY_CHECKS "llvm-include-order")
#
# set(CMAKE_CXX_CLANG_TIDY 
#     clang-tidy;
#     -extra-arg=-std=c++2b;
#     -extra-arg=-Wall;
#     -extra-arg=-Wpedantic;
#     -extra-arg=-stdlib=libc++;
#     ${CLANG_TIDY_CHECKS}
# )

add_subdirectory(messages)
include_directories(${PROTO_OUTPUT_DIR})

include_directories(${PROJECT_SOURCE_DIR}/lib)
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/nodes)

add_subdirectory(include)
add_subdirectory(src)
add_subdirectory(lib)
add_subdirectory(nodes)


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

add_custom_target(scan
	COMMAND scan-build make
	WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
	COMMENT "Scans the project with clang"
)
# add_dependencies(scan ${PROJECT_NAME})

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

# Include what you use
# option(USE_IWYU "Use include what you use executable in the building process" OFF) 
# if(USE_IWYU)
# 	message("Option set IWYU")
# 	find_program(iwyu_path NAMES include-what-you-use iwyu)
# 	if(NOT iwyu_path)
# 		message("${Yellow}Include what you use executable not found${Reset}")
# 	else()
# 		message("${Green}Include what you use executable found${Reset}")
# 		set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_INCLUDE_WHAT_YOU_USE ${iwyu_path})
# 	endif()
# endif()
