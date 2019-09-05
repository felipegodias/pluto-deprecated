#pragma once

#include "pluto/service/base_service.h"

namespace pluto
{
    class PLUTO_API RenderManager : public BaseService
    {
    public:
        virtual ~RenderManager() = 0;

        RenderManager();

        RenderManager(const RenderManager& other) = delete;
        RenderManager(RenderManager&& other) noexcept;
        RenderManager& operator=(const RenderManager& rhs) = delete;
        RenderManager& operator=(RenderManager&& rhs) noexcept;

        virtual void MainLoop() = 0;
    };
}
