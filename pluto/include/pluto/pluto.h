#pragma once

#include "pluto/asset/asset.h"
#include "pluto/asset/asset_manager.h"
#include "pluto/asset/font_asset.h"
#include "pluto/asset/material_asset.h"
#include "pluto/asset/mesh_asset.h"
#include "pluto/asset/package_manifest_asset.h"
#include "pluto/asset/shader_asset.h"
#include "pluto/asset/text_asset.h"
#include "pluto/asset/texture_asset.h"

#include "pluto/config/config_manager.h"

#include "pluto/event/base_event.h"
#include "pluto/event/event_manager.h"

#include "pluto/file/file_manager.h"
#include "pluto/file/file_reader.h"
#include "pluto/file/file_writer.h"
#include "pluto/file/path.h"

#include "pluto/input/input_manager.h"
#include "pluto/input/key_code.h"

#include "pluto/log/log_manager.h"

#include "pluto/math/bounds.h"
#include "pluto/math/color.h"
#include "pluto/math/matrix2x2.h"
#include "pluto/math/matrix3x3.h"
#include "pluto/math/matrix4x4.h"
#include "pluto/math/quaternion.h"
#include "pluto/math/vector2f.h"
#include "pluto/math/vector2i.h"
#include "pluto/math/vector3f.h"
#include "pluto/math/vector3i.h"
#include "pluto/math/vector4f.h"
#include "pluto/math/vector4i.h"

#include "pluto/memory/object.h"
#include "pluto/memory/resource.h"

#include "pluto/scene/game_object.h"
#include "pluto/scene/scene.h"
#include "pluto/scene/scene_manager.h"
#include "pluto/scene/components/camera.h"
#include "pluto/scene/components/component.h"
#include "pluto/scene/components/mesh_renderer.h"
#include "pluto/scene/components/renderer.h"
#include "pluto/scene/components/transform.h"

#include "pluto/service/base_factory.h"
#include "pluto/service/base_service.h"
#include "pluto/service/service_collection.h"

#include "pluto/simulation/simulation_manager.h"

#include "pluto/exception.h"
#include "pluto/guid.h"
#include "pluto/regex.h"
#include "pluto/root.h"
#include "pluto/stop_watch.h"
