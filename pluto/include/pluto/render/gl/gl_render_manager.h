#pragma once

#include "pluto/render/render_manager.h"
#include "pluto/di/base_factory.h"

#include <memory>

namespace pluto
{
    class PLUTO_API GlRenderManager final : public RenderManager
    {
    public:
        class PLUTO_API Factory final : public BaseFactory
        {
        public:
            explicit Factory(DiContainer& diContainer);
            std::unique_ptr<GlRenderManager> Create() const;
        };

    private:
        class PLUTO_API Impl;
        std::unique_ptr<Impl> impl;

    public:
        explicit GlRenderManager(std::unique_ptr<Impl> impl);
        GlRenderManager(const GlRenderManager& other) = delete;
        GlRenderManager(GlRenderManager&& other) noexcept;

        ~GlRenderManager() override;

        GlRenderManager& operator=(const GlRenderManager& rhs) = delete;
        GlRenderManager& operator=(GlRenderManager&& rhs) noexcept;
    };
}