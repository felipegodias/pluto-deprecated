project(pluto_asset_manager CXX)

list(APPEND ASSET_MANAGER_SOURCE_FILES
    # .
    ${CMAKE_CURRENT_SOURCE_DIR}/base_compiler.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    # ./compilers
    ${CMAKE_CURRENT_SOURCE_DIR}/compilers/font_compiler.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/compilers/material_compiler.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/compilers/mesh_compiler.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/compilers/package_compiler.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/compilers/shader_compiler.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/compilers/text_compiler.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/compilers/texture_compiler.cpp
    # ./dummy
    ${CMAKE_CURRENT_SOURCE_DIR}/dummy/dummy_shader_program.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dummy/dummy_texture_buffer.cpp
)

add_executable(pluto_asset_manager ${ASSET_MANAGER_SOURCE_FILES})


target_link_libraries(pluto_asset_manager PRIVATE pluto)

set_target_properties(pluto_asset_manager PROPERTIES
    CXX_STANDARD 17
    CXX_EXTENSIONS OFF
)