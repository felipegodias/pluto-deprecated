#pragma once

#include "pluto/scene/component.h"

namespace pluto
{
    class Transform;
    class MeshAsset;
    class MaterialAsset;
    class Bounds;

    class PLUTO_API Renderer : public Component
    {
    public:
        Renderer();
        Renderer(const Renderer& other) = delete;

        virtual ~Renderer() = 0;
        Component& operator=(const Renderer& rhs) = delete;

        virtual Bounds GetBounds() = 0;

        virtual Transform& GetTransform() const = 0;
        virtual MeshAsset& GetMesh() const = 0;
        virtual MaterialAsset& GetMaterial() const = 0;
    };
}
