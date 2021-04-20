set_submodule_library(Com QssEditor)
target_link_libraries(${CURRENT_MODULE_TARGET}
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        Qt5::Designer
        )
set_simple_meta_info()