function(set_build_time_header)
    set(options EACH_BUILD)
    set(oneValueArgs PATH TARGET)
    set(multiValueArgs)
    cmake_parse_arguments(
            ARG
            "${options}"
            "${oneValueArgs}"
            "${multiValueArgs}"
            ${ARGN}
    )
    if (NOT ARG_TARGET)
        message(FATAL_ERROR "Must specify TARGET argument")
    endif ()
    if (NOT ARG_PATH)
        set(ARG_PATH ${CMAKE_BINARY_DIR}/build_time.h)
        message(STATUS "FILE_PATH not set, use FILE_PATH=${ARG_PATH}")
    endif ()

#    cmake_print_variables(ARG_EACH_BUILD)

    execute_process(COMMAND ${CMAKE_COMMAND}
            -DBUILD_TIME_HEADER:PATH=${ARG_PATH}
            -P "${CMAKE_SOURCE_DIR}/cmake/UpdateBuildTimeScript.cmake")

    add_custom_target(UpdateBuildTime
            COMMAND ${CMAKE_COMMAND}
            -DBUILD_TIME_HEADER:PATH="${ARG_PATH}"
            -P "${CMAKE_SOURCE_DIR}/cmake/UpdateBuildTimeScript.cmake")

    if (ARG_EACH_BUILD)
        add_dependencies(${ARG_TARGET} UpdateBuildTime)
    endif ()
    get_filename_component(BUILD_TIME_HEADER_DIR ${ARG_PATH} DIRECTORY)
    target_include_directories(${ARG_TARGET} PRIVATE ${BUILD_TIME_HEADER_DIR})
endfunction()
