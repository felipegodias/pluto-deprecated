list(APPEND PLUTO_SOURCE_FILES
    # .
    ${CMAKE_CURRENT_SOURCE_DIR}/exception.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/guid.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/regex.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/root.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/stack_trace.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/stop_watch.cpp
    # ./asset
    ${CMAKE_CURRENT_SOURCE_DIR}/asset/asset.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/asset/asset_installer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/asset/asset_manager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/asset/font_asset.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/asset/material_asset.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/asset/mesh_asset.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/asset/package_manifest_asset.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/asset/shader_asset.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/asset/text_asset.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/asset/texture_asset.cpp
    # ./asset/events
    ${CMAKE_CURRENT_SOURCE_DIR}/asset/events/on_asset_unload_event.cpp
    # ./event
    ${CMAKE_CURRENT_SOURCE_DIR}/event/base_event.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/event/event_installer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/event/event_manager.cpp
    # ./config
    ${CMAKE_CURRENT_SOURCE_DIR}/config/config_installer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/config/config_manager.cpp
    # ./debug
    ${CMAKE_CURRENT_SOURCE_DIR}/debug/assert.cpp
    # ./file
    ${CMAKE_CURRENT_SOURCE_DIR}/file/file_manager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/file/file_reader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/file/file_writer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/file/path.cpp
    # ./input
    ${CMAKE_CURRENT_SOURCE_DIR}/input/input_installer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/input/input_manager.cpp
    # ./log
    ${CMAKE_CURRENT_SOURCE_DIR}/log/log_installer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/log/log_manager.cpp
    # ./math
    ${CMAKE_CURRENT_SOURCE_DIR}/math/bounds.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/math/color.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/math/matrix2x2.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/math/matrix3x3.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/math/matrix4x4.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/math/quaternion.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/math/rect.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/math/vector2f.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/math/vector2i.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/math/vector3f.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/math/vector3i.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/math/vector4f.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/math/vector4i.cpp
    # ./memory
    ${CMAKE_CURRENT_SOURCE_DIR}/memory/memory_installer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/memory/memory_manager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/memory/object.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/memory/resource.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/memory/resource_control.cpp
    # ./physics_2d
    ${CMAKE_CURRENT_SOURCE_DIR}/physics_2d/physics_2d_manager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/physics_2d/physics_2d_body.cpp
    # ./physics_2d/shapes
    ${CMAKE_CURRENT_SOURCE_DIR}/physics_2d/shapes/physics_2d_circle_shape.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/physics_2d/shapes/physics_2d_shape.cpp
    # ./render
    ${CMAKE_CURRENT_SOURCE_DIR}/render/mesh_buffer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/render/render_manager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/render/render_installer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/render/shader_program.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/render/texture_buffer.cpp
    # ./render/gl
    ${CMAKE_CURRENT_SOURCE_DIR}/render/gl/gl_call.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/render/gl/gl_mesh_buffer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/render/gl/gl_render_manager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/render/gl/gl_shader_program.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/render/gl/gl_texture_buffer.cpp
    # ./scene
    ${CMAKE_CURRENT_SOURCE_DIR}/scene/game_object.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/scene/scene.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/scene/scene_installer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/scene/scene_manager.cpp
    # ./scene/components
    ${CMAKE_CURRENT_SOURCE_DIR}/scene/components/camera.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/scene/components/component.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/scene/components/mesh_renderer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/scene/components/renderer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/scene/components/text_renderer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/scene/components/transform.cpp
    # ./service
    ${CMAKE_CURRENT_SOURCE_DIR}/service/base_factory.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/service/base_service.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/service/service_collection.cpp
    # ./simulation
    ${CMAKE_CURRENT_SOURCE_DIR}/simulation/simulation_installer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/simulation/simulation_manager.cpp
    # ./window
    ${CMAKE_CURRENT_SOURCE_DIR}/window/window_installer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/window/window_manager.cpp
)

set(PLUTO_SOURCE_FILES ${PLUTO_SOURCE_FILES} PARENT_SCOPE)