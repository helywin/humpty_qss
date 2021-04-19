set(qcodeeditor_DIR ${CMAKE_CURRENT_SOURCE_DIR}/QCodeEditor)
set(qcodeeditor_INC_DIR ${qcodeeditor_DIR}/include/internal)
set(qcodeeditor_LIB Com.QCodeEditor)

file(GLOB SRCS
        ${qcodeeditor_DIR}/src/internal/*.cpp
        ${qcodeeditor_INC_DIR}/*.hpp
        ${qcodeeditor_DIR}/resources/qcodeeditor_resources.qrc
        )

add_library(${qcodeeditor_LIB} STATIC ${SRCS})
target_link_libraries(${qcodeeditor_LIB}
        Qt5::Core
        Qt5::Widgets
        Qt5::Gui
        )
target_include_directories(${qcodeeditor_LIB} PRIVATE ${qcodeeditor_INC_DIR})
target_include_directories(${qcodeeditor_LIB} PUBLIC ${qcodeeditor_DIR}/include)