set_submodule_library(Com Settings)
target_link_libraries(${CURRENT_MODULE_TARGET}
        Qt${QT_VERSION}::Core)
set_simple_meta_info()