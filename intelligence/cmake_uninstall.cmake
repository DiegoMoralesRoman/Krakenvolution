set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
if(NOT EXISTS "${CMAKE_INSTALL_MANIFEST}")
    message(FATAL_ERROR "Cannot find install manifest: ${CMAKE_INSTALL_MANIFEST}")
endif()

file(READ "${CMAKE_INSTALL_MANIFEST}" files)
string(REGEX REPLACE "\n" ";" files "${files}")
foreach(file ${files})
    message(STATUS "Uninstalling \"${file}\"")
    if(EXISTS "${file}")
        execute_process(
            COMMAND "${CMAKE_COMMAND}" -E remove "${file}"
            OUTPUT_VARIABLE rm_out
            RESULT_VARIABLE rm_retval
        )
        if(NOT rm_retval EQUAL 0)
            message(FATAL_ERROR "Problem when removing \"${file}\"")
        endif()
    else()
        message(STATUS "File \"${file}\" does not exist.")
    endif()
endforeach()
