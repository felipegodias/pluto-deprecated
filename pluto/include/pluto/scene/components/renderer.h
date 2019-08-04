#pragma once

#include "component.h"

namespace pluto
{
    template <typename T, typename Enable = void>
    class Resource;

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
        virtual Resource<MeshAsset> GetMesh() const = 0;
        virtual Resource<MaterialAsset> GetMaterial() const = 0;
    };
}
