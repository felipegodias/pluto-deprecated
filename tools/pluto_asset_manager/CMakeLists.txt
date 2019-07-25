project(pluto_asset_manager CXX)

list(APPEND ASSET_MANAGER_SOURCE_FILES
    # .
    ${CMAKE_CURRENT_SOURCE_DIR}/asset_dumper.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/base_menu.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/menu_options.cpp
    # ./mesh
    ${CMAKE_CURRENT_SOURCE_DIR}/mesh/mesh_asset_manager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/mesh/mesh_asset_menu.cpp
    # ./package
    ${CMAKE_CURRENT_SOURCE_DIR}/package/package_manager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/package/package_menu.cpp
    # ./shader
    ${CMAKE_CURRENT_SOURCE_DIR}/shader/shader_asset_manager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/shader/shader_asset_menu.cpp
    # ./text
    ${CMAKE_CURRENT_SOURCE_DIR}/text/text_asset_manager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/text/text_asset_menu.cpp
    # ./texture
    ${CMAKE_CURRENT_SOURCE_DIR}/texture/dummy_texture_buffer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/texture/texture_asset_manager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/texture/texture_asset_menu.cpp
)

add_executable(pluto_asset_manager ${ASSET_MANAGER_SOURCE_FILES})


target_link_libraries(pluto_asset_manager PRIVATE pluto)

set_target_properties(pluto_asset_manager PROPERTIES
    CXX_STANDARD 17
    CXX_EXTENSIONS OFF
)