#pragma once

#include "pluto/service/base_service.h"

#include <vector>

namespace pluto
{
    class Vector2F;
    class Color;

    class PLUTO_API RenderManager : public BaseService
    {
    public:
        virtual ~RenderManager() = 0;

        RenderManager();

        RenderManager(const RenderManager& other) = delete;
        RenderManager(RenderManager&& other) noexcept;
        RenderManager& operator=(const RenderManager& rhs) = delete;
        RenderManager& operator=(RenderManager&& rhs) noexcept;

        virtual void DrawCircleGizmo(const Vector2F& position, float radius, const Color& color) = 0;
        virtual void DrawPolygonGizmo(const std::vector<Vector2F>& points, const Color& color) = 0;
        virtual void DrawLineGizmo(const Vector2F& from, const Vector2F& to, const Color& color) = 0;
    };
}
