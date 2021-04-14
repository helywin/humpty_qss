set_submodule_library(Com CrashHandler)
target_link_libraries(${CURRENT_MODULE_TARGET}
        Qt5::Core
        Qt5::Widgets
        Com.Logger
        )
set_simple_meta_info()
