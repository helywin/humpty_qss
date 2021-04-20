cmake_minimum_required(VERSION 3.15)
project(humpty_qss)

set(CMAKE_CXX_STANDARD 17)
#set(PRODUCT_NAME "Solar Profiler")
#set(COPY_RIGHT "copyright 2021")
#set(LEGAL_TRADEMARKS "Solar")

if (NOT DEFINED Qt5_SDK_DIR)
    set(Qt5_SDK_DIR D:/Qt/5.15.2/msvc2019_64)
#    set(Qt5_DIR D:/Qt/5.15.2/msvc2019_64/lib/cmake/Qt5)
endif ()

list(APPEND CMAKE_PREFIX_PATH ${Qt5_SDK_DIR})
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

include(ReadVersion)
include(Submodule)
include(UpdateBuildTime)
include(GenerateQrc)

set(QT_VERSION 5)
find_package(Qt${QT_VERSION} COMPONENTS Core Widgets Gui PrintSupport Designer REQUIRED)

if (NOT DEFINED CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Release or Debug?" FORCE)
endif ()

if (CMAKE_BUILD_TYPE MATCHES Debug)
    message("=== Debug Mode ===")
    set(PROJECT_BINARY_DIR ${PROJECT_SOURCE_DIR}/debug)
    set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/debug)
    add_definitions(-DDEBUG) # 加此宏是为了激活代码里面有的#ifdef DEBUG宏
elseif (CMAKE_BUILD_TYPE MATCHES Release) #CMAKE_BUILD_TYPE MATCHES Release
    message("=== Release Mode ===")
    set(PROJECT_BINARY_DIR ${PROJECT_SOURCE_DIR}/release)
    set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/release)
    # 屏蔽Qt警告和调试
    add_definitions(-DQT_NO_DEBUG)
    add_definitions(-DQT_NO_DEBUG_OUTPUT)
    add_definitions(-DQT_DEPRECATED_WARNINGS)
endif ()

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR})

option(BUILD_UNICODE "Option for build with unicode api" ON)
if (BUILD_UNICODE)
    set(UNICODE_FLAGS "-DUNICODE -D_UNICODE")
endif ()

# 设置编译参数
if (UNIX)
    set(DEFINES "-Wunused-parameter")
    set(CMAKE_CXX_FLAGS_DEBUG "-pipe -O2 -std=gnu++11 -Wall -W -D_REENTRANT -fPIC ${DEFINES}")
    set(CMAKE_C_FLAGS_DEBUG "-pipe -O2 -Wall -W -D_REENTRANT -fPIC ${DEFINES}")
    set(CMAKE_EXE_LINKER_FLAGS_DEBUG "-fno-pie")

    set(CMAKE_CXX_FLAGS_RELEASE "-pipe -O2 -std=gnu++11 -Wall -W -D_REENTRANT -fPIC ${DEFINES}")
    set(CMAKE_C_FLAGS_RELEASE "-pipe -O2 -Wall -W -D_REENTRANT -fPIC ${DEFINES}")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "-fno-pie")
elseif (WIN32)
    if (MSVC)
        set(DEFINES_DEBUG "${UNICODE_FLAGS} -D_WINDOWS -DWIN32 -D_ENABLE_EXTENDED_ALIGNED_STORAGE -DWIN64 /wd4100")
        # /analyze
        set(CMAKE_CXX_FLAGS_DEBUG "/utf-8 /sdl /EHsc /D_DEBUG -nologo -Zc:wchar_t -FS -Zc:strictStrings -Zi -MDd -W3 -w44456 -w44457 -w44458 ${DEFINES_DEBUG}")
        set(CMAKE_C_FLAGS_DEBUG "/utf-8 /sdl /D_DEBUG -nologo -Zc:wchar_t -FS -Zc:rvalueCast -Zc:inline -Zc:strictStrings -Zc:throwingNew -Zc:referenceBinding -Zc:__cplusplus /FS -Zi -MDd -W3 -w34100 -w34189 -w44996 -w44456 -w44457 -w44458 -wd4577 -wd4467 -EHsc ${DEFINES_DEBUG}")
        set(CMAKE_EXE_LINKER_FLAGS_DEBUG "/NOLOGO /DYNAMICBASE /NXCOMPAT /INCREMENTAL:NO /DEBUG /IGNORE:4099")
        set(CMAKE_SHARED_LINKER_FLAGS_DEBUG "/NOLOGO /DYNAMICBASE /NXCOMPAT /DEBUG /DLL /SUBSYSTEM:WINDOWS /IGNORE:4099")

        if (ASAN)
            message(STATUS "build with asan")
            set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /fsanitize=address")
            set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /fsanitize=address")
            set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} /WHOLEARCHIVE:clang_rt.asan_dbg_dynamic-x86_64.lib /WHOLEARCHIVE:clang_rt.asan_dbg_dynamic_runtime_thunk-x86_64.lib /WHOLEARCHIVE:vcasand.lib")
            set(CMAKE_SHARED_LINKER_FLAGS_DEBUG "${CMAKE_SHARED_LINKER_FLAGS_DEBUG} /WHOLEARCHIVE:clang_rt.asan_dbg_dynamic-x86_64.lib /WHOLEARCHIVE:clang_rt.asan_dbg_dynamic_runtime_thunk-x86_64.lib /WHOLEARCHIVE:vcasand.lib")
            #/WHOLEACHIVE:clang_rt.asan_dbg_dll_thunk-x86_64.lib
            #/WHOLEARCHIVE:clang_rt.asan_cxx_dbg-x86_64.lib
            #
        endif ()

        # release模式下生成pdb用于调试
        set(DEFINES_RELEASE "${UNICODE_FLAGS} -D_WINDOWS -DWIN32 -D_ENABLE_EXTENDED_ALIGNED_STORAGE -DWIN64")
        set(CMAKE_C_FLAGS_RELEASE "/utf-8 /sdl -nologo -Zc:wchar_t -FS -Zi -Zc:strictStrings -O2 -MD -W3 -w44456 -w44457 -w44458 ${DEFINES_RELEASE}")
        set(CMAKE_CXX_FLAGS_RELEASE "/utf-8 /sdl -nologo -Zc:wchar_t -FS -Zi -Zc:rvalueCast -Zc:inline -Zc:strictStrings -Zc:throwingNew -Zc:referenceBinding -Zc:__cplusplus -O2 -MD -W3 -w34100 -w34189 -w44996 -w44456 -w44457 -w44458 -wd4577 -wd4467 -EHsc ${DEFINES_RELEASE}")
        set(CMAKE_EXE_LINKER_FLAGS_RELEASE "/NOLOGO /DYNAMICBASE /NXCOMPAT /INCREMENTAL:NO /DEBUG /OPT:REF /OPT:ICF")# /NODEFAULTLIB:MSVCRT /NODEFAULTLIB:LIBC
        set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "/NOLOGO /DYNAMICBASE /NXCOMPAT /INCREMENTAL:NO /DLL /SUBSYSTEM:WINDOWS /DEBUG /OPT:REF /OPT:ICF")
    else ()
        message(FATAL_ERROR "platform not supported ")
    endif ()
endif ()

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

generate_version_file(APP_NAME HumptyQss APP_ORG Helywin)

add_subdirectory(third_party)
add_subdirectory(src)
add_subdirectory(test)