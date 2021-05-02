set_submodule_executable(HumptyQss Gui)
set(MODULE_LIBS
        Com.Logger
        Com.Utils
        Com.Translatable
        Com.QssEditor
        )
if (WIN32)
    list(APPEND MODULE_LIBS Com.CrashHandler)
endif ()
target_link_libraries(${CURRENT_MODULE_TARGET}
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        ${MODULE_LIBS}
        )
if (WIN32)
    target_link_libraries(${CURRENT_MODULE_TARGET}
            dbghelp)
endif ()
set(ICON_PATH ${CMAKE_SOURCE_DIR}/res/solar.ico)
string(REPLACE "/" "\\\\" ICON_PATH_ESCAPED ${ICON_PATH})
generate_meta_info(
        META_FILE
        MODULE_NAME ${CURRENT_MODULE_NAME}
        PRODUCT_NAME ${CURRENT_PREFIX_NAME}
        ORIGINAL_FILE_NAME ${CURRENT_PREFIX_NAME}.${CURRENT_MODULE_NAME}
        PRODUCT_VERSION ${GENERAL_VERSION}
        ICON "${ICON_PATH_ESCAPED}")
target_sources(${CURRENT_MODULE_TARGET} PRIVATE ${META_FILE})
generate_qrc()
#set_property(TARGET ${CURRENT_MODULE_TARGET} PROPERTY
#        MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

#cmake_print_properties(TARGETS ${CURRENT_MODULE_TARGET} PROPERTIES LINK_LIBRARIES)

#get_target_property(LIBS ${CURRENT_MODULE_TARGET} LINK_LIBRARIES)

install(TARGETS ${CURRENT_MODULE_TARGET} ${MODULE_LIBS}
        CONFIGURATIONS Debug
        RUNTIME DESTINATION ${CMAKE_SOURCE_DIR}/bin/humpty_qss_dbg
        LIBRARY DESTINATION ${CMAKE_SOURCE_DIR}/bin/humpty_qss_dbg
        )

install(TARGETS ${CURRENT_MODULE_TARGET} ${MODULE_LIBS}
        CONFIGURATIONS Release
        RUNTIME DESTINATION ${CMAKE_SOURCE_DIR}/bin/humpty_qss
        LIBRARY DESTINATION ${CMAKE_SOURCE_DIR}/bin/humpty_qss
        )