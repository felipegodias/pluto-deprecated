#include "pluto/render/render_manager.h"

namespace pluto
{
    RenderManager::~RenderManager() = default;

    RenderManager::RenderManager() = default;

    RenderManager::RenderManager(RenderManager&& other) noexcept = default;

    RenderManager& RenderManager::operator=(RenderManager&& rhs) noexcept = default;
}
