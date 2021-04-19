set_submodule_library(Com Utils)
target_link_libraries(${CURRENT_MODULE_TARGET}
        Qt5::Widgets
        Qt5::Core
        Qt5::Gui
        )
set_simple_meta_info()
