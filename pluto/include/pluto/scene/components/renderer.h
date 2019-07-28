#pragma once

#include "component.h"

namespace pluto
{
    class Transform;
    class MeshAsset;
    class MaterialAsset;
    class Bounds;

    class PLUTO_API Renderer : public Component
    {
    public:
        virtual ~Renderer() = 0;
        Renderer();

        Renderer(const Renderer& other) = delete;
        Renderer(Renderer&& other) noexcept;
        Renderer& operator=(const Renderer& rhs) = delete;
        Renderer& operator=(Renderer&& rhs) noexcept;

        virtual Bounds GetBounds() = 0;

        virtual Transform& GetTransform() const = 0;
        virtual MeshAsset& GetMesh() const = 0;
        virtual MaterialAsset& GetMaterial() const = 0;
    };
}
