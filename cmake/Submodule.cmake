include_guard()
include(CMakePrintHelpers)
include(GenerateExportHeader)
include(GenerateMetaInfo)
include(ReadVersion)
set(MODULE_LIST "")

macro(append_module_list)
    set(MODULE_INFO "${CURRENT_MODULE_NAME},${CURRENT_MODULE_TARGET},${CURRENT_MODULE_TYPE}")
    message(STATUS "Add module: ${MODULE_INFO}")
    list(APPEND MODULE_LIST ${MODULE_INFO})
    unset(MODULE_INFO)
endmacro()

macro(set_submodule_library PREFIX_NAME_IN MODULE_NAME_IN)
    set(CURRENT_MODULE_NAME "${MODULE_NAME_IN}")
    set(CURRENT_PREFIX_NAME "${PREFIX_NAME_IN}")
    set(CURRENT_MODULE_TYPE "LIBRARY")
    set(CURRENT_MODULE_TARGET ${CURRENT_PREFIX_NAME}.${CURRENT_MODULE_NAME})
    project(${CURRENT_MODULE_TARGET})
    file(GLOB SRCS
            ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/*.h
            ${CMAKE_CURRENT_SOURCE_DIR}/*.c
            )
    add_library(${CURRENT_MODULE_TARGET} SHARED ${SRCS})
    set_target_properties(${CURRENT_MODULE_TARGET} PROPERTIES
            OUTPUT_NAME ${CURRENT_MODULE_TARGET}
            DEBUG_POSTFIX d
            NAMESPACE ${CURRENT_PREFIX_NAME}
            )
    target_include_directories(${CURRENT_MODULE_TARGET} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
    target_include_directories(${CURRENT_MODULE_TARGET} PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
    target_include_directories(${CURRENT_MODULE_TARGET} PUBLIC ${CMAKE_BINARY_DIR})
    string(TOUPPER ${CURRENT_MODULE_NAME} MODULE_NAME_UPPER)
    string(TOUPPER ${CURRENT_PREFIX_NAME} PREFIX_NAME_UPPER)
    generate_export_header(${CURRENT_MODULE_TARGET}
            BASE_NAME ${CURRENT_MODULE_NAME}
            EXPORT_MACRO_NAME ${PREFIX_NAME_UPPER}_${MODULE_NAME_UPPER}_API
            EXPORT_FILE_NAME ${CURRENT_MODULE_NAME}_Export.hpp
            STATIC_DEFINE ${MODULE_NAME_UPPER}_BUILT_AS_STATIC
            )
    append_module_list()
endmacro()

macro(set_submodule_executable PREFIX_NAME_IN MODULE_NAME_IN)
    set(CURRENT_MODULE_NAME "${MODULE_NAME_IN}")
    set(CURRENT_PREFIX_NAME "${PREFIX_NAME_IN}")
    set(CURRENT_MODULE_TYPE "EXECUTABLE")
    set(CURRENT_MODULE_TARGET ${CURRENT_PREFIX_NAME}.${CURRENT_MODULE_NAME})
    project(${CURRENT_MODULE_TARGET})
    file(GLOB SRCS
            ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/*.h
            ${CMAKE_CURRENT_SOURCE_DIR}/*.c
            )
    add_executable(${CURRENT_MODULE_TARGET} ${SRCS})
    set_target_properties(${CURRENT_MODULE_TARGET} PROPERTIES
            OUTPUT_NAME ${CURRENT_MODULE_TARGET}
            DEBUG_POSTFIX ""
            NAMESPACE ${CURRENT_PREFIX_NAME}
            )
    target_include_directories(${CURRENT_MODULE_TARGET} PUBLIC ${CMAKE_BINARY_DIR})
    append_module_list()
endmacro()

macro(set_simple_meta_info)
    if (WIN32)
        generate_meta_info(
                META_FILE
                MODULE_NAME ${CURRENT_MODULE_NAME}
                PRODUCT_NAME ${CURRENT_PREFIX_NAME}
                ORIGINAL_FILE_NAME ${CURRENT_PREFIX_NAME}.${CURRENT_MODULE_NAME}
                PRODUCT_VERSION ${GENERAL_VERSION})
        target_sources(${CURRENT_MODULE_TARGET} PRIVATE ${META_FILE})
    endif ()
endmacro()

function(is_submodule)
    if (DEFINED CURRENT_MODULE_TYPE)
        return(true)
    else ()
        return(false)
    endif ()
endfunction()

function(check_submodule)
    if (NOT DEFINED CURRENT_MODULE_TYPE)
        message(FATAL_ERROR "submodule is not exist")
    endif ()
    if (CURRENT_MODULE_NAME STREQUAL "")
        message(FATAL_ERROR "CURRENT_MODULE_NAME is empty string")
    endif ()

    if (CURRENT_PREFIX_NAME STREQUAL "")
        message(FATAL_ERROR "CURRENT_PREFIX_NAME is empty string")
    endif ()

    if (CURRENT_MODULE_TYPE STREQUAL "")
        message(FATAL_ERROR "CURRENT_MODULE_TYPE is empty string")
    endif ()

    if (CURRENT_MODULE_TARGET STREQUAL "")
        message(FATAL_ERROR "CURRENT_MODULE_TARGET is empty string")
    endif ()
endfunction()