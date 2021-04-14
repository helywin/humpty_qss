include_guard()
include(FindGit)
if (${GIT_FOUND})
    message(STATUS "Found Git executable, version: ${GIT_VERSION_STRING}")
else ()
    message(WARNING "Cannot find Git executable, skip git version check")
endif ()

function(get_git_info GIT_COMMIT GIT_BRANCH)
    if (NOT GIT_FOUND)
        set(${GIT_COMMIT} "unknown" PARENT_SCOPE)
        set(${GIT_BRANCH} "unknown" PARENT_SCOPE)
#        set(GIT_STATUS "")
        return()
    endif ()
    execute_process(
            COMMAND ${GIT_EXECUTABLE} describe --tags --exact-match --abbrev=0
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
            OUTPUT_VARIABLE GIT_TAG_INFO
            OUTPUT_STRIP_TRAILING_WHITESPACE
            RESULT_VARIABLE GIT_VERSION_RESULT
    )
    if (NOT ${GIT_VERSION_RESULT} EQUAL 0)
        set(GIT_STATUS "u")
        set(GIT_TAG "notag")
    else ()
        set(GIT_VERSION_STATUS "")
    endif ()

    execute_process(
            COMMAND ${GIT_EXECUTABLE} log -1 --format=%H
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
            OUTPUT_VARIABLE GIT_COMMIT_INFO
            OUTPUT_STRIP_TRAILING_WHITESPACE
            RESULT_VARIABLE GIT_COMMIT_RESULT
    )
    if (NOT GIT_COMMIT_RESULT EQUAL 0)
        message(FATAL_ERROR "cannot find git commit number")
    else ()
        message(STATUS "git commit: ${GIT_COMMIT_INFO}")
    endif ()

    execute_process(
            COMMAND ${GIT_EXECUTABLE} rev-parse --abbrev-ref HEAD
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
            OUTPUT_VARIABLE GIT_BRANCH_INFO
            OUTPUT_STRIP_TRAILING_WHITESPACE
            RESULT_VARIABLE GIT_BRANCH_RESULT
    )
    if (NOT GIT_BRANCH_RESULT EQUAL 0)
        message(WARNING "cannot find git branch name")
    else ()
        message(STATUS "git branch: ${GIT_BRANCH_INFO}")
    endif ()

    set(${GIT_STATUS} ${GIT_STATUS_INFO} PARENT_SCOPE)
    set(${GIT_COMMIT} ${GIT_COMMIT_INFO} PARENT_SCOPE)
    set(${GIT_BRANCH} ${GIT_BRANCH_INFO} PARENT_SCOPE)
endfunction()