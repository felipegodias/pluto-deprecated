project(pluto_asset_manager CXX)

add_executable(pluto_asset_manager main.cpp text_asset_menu.cpp text_asset_manager.cpp)

target_link_libraries(pluto_asset_manager PRIVATE pluto)

set_target_properties(pluto_asset_manager PROPERTIES
    CXX_STANDARD 17
    CXX_EXTENSIONS OFF
)