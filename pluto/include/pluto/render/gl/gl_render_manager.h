#pragma once

#include "pluto/render/render_manager.h"
#include "pluto/service/base_factory.h"

#include <memory>

namespace pluto
{
    class PLUTO_API GlRenderManager final : public RenderManager
    {
    public:
        class PLUTO_API Factory final : public BaseFactory
        {
        public:
            explicit Factory(ServiceCollection& serviceCollection);
            std::unique_ptr<GlRenderManager> Create() const;
        };

    private:
        class PLUTO_API Impl;
        std::unique_ptr<Impl> impl;

    public:
        ~GlRenderManager() override;
        explicit GlRenderManager(std::unique_ptr<Impl> impl);

        GlRenderManager(const GlRenderManager& other) = delete;
        GlRenderManager(GlRenderManager&& other) noexcept;
        GlRenderManager& operator=(const GlRenderManager& rhs) = delete;
        GlRenderManager& operator=(GlRenderManager&& rhs) noexcept;

        void MainLoop() override;
    };
}
