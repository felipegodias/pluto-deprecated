#pragma once

#include "pluto/scene/components/renderer.h"

#include <memory>

namespace pluto
{
    class PLUTO_API MeshRenderer final : public Renderer
    {
    public:
        class PLUTO_API Factory final : public Component::Factory
        {
        public:
            explicit Factory(ServiceCollection& serviceCollection);
            std::unique_ptr<Component> Create(const Resource<GameObject>& gameObject) const override;
        };

    private:
        class PLUTO_API Impl;
        std::unique_ptr<Impl> impl;

    public:
        ~MeshRenderer() override;
        explicit MeshRenderer(std::unique_ptr<Impl> impl);

        MeshRenderer(const MeshRenderer& other) = delete;
        MeshRenderer(MeshRenderer&& other) noexcept;
        MeshRenderer& operator=(const MeshRenderer& rhs) = delete;
        MeshRenderer& operator=(MeshRenderer&& rhs) noexcept;

        Bounds GetBounds() override;

        Resource<MeshAsset> GetMesh() const override;
        void SetMesh(const Resource<MeshAsset>& value);

        Resource<MaterialAsset> GetMaterial() const override;
        void SetMaterial(const Resource<MaterialAsset>& value);
    };
}
