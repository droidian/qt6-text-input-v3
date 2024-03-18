cmake_minimum_required(VERSION 3.16)
project(qt6-text-input-v3 VERSION 0.1)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt6 REQUIRED COMPONENTS Core WaylandClient)

set(plugin_SRCS
    src/waylandtextinputv3contextplugin.cpp
    src/waylandtextinputv3context.cpp
    src/qwaylandtextinputv3.cpp
    src/qwaylandinputmethodeventbuilder.cpp
)

qt6_add_library(textinputv3platforminputcontextplugin MODULE ${plugin_SRCS})
set_target_properties(textinputv3platforminputcontextplugin PROPERTIES
                         AUTOMOC TRUE
                        )

qt6_generate_wayland_protocol_client_sources(
    textinputv3platforminputcontextplugin
    FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/protocols/text-input-unstable-v3.xml
)

target_link_libraries(textinputv3platforminputcontextplugin
                          Qt6::Core
                          Qt6::Gui
                          Qt6::GuiPrivate
                          Qt6::WaylandClient
                          Qt6::WaylandClientPrivate
                         )

install(TARGETS textinputv3platforminputcontextplugin DESTINATION ${CMAKE_INSTALL_LIBDIR}/qt6/plugins/platforminputcontexts)