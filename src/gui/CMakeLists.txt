set_submodule_executable(HumptyQss Gui)
target_link_libraries(${CURRENT_MODULE_TARGET}
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        Com.CrashHandler
        Com.Logger
        Com.Utils
        Com.Translatable
        Com.QssEditor
        )
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
