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
            ~Factory() override;
            explicit Factory(ServiceCollection& diContainer);

            Factory(const Factory& other) = delete;
            Factory(Factory&& other) noexcept;
            Factory& operator=(const Factory& rhs) = delete;
            Factory& operator=(Factory&& rhs) noexcept;

            std::unique_ptr<Component> Create(GameObject& gameObject) const override;
        };

    private:
        class PLUTO_API Impl;
        std::unique_ptr<Impl> impl;

    public:
        explicit MeshRenderer(std::unique_ptr<Impl> impl);
        MeshRenderer(const MeshRenderer& other) = delete;
        MeshRenderer(MeshRenderer&& other) noexcept;
        ~MeshRenderer();

        MeshRenderer& operator=(const MeshRenderer& rhs) = delete;
        MeshRenderer& operator=(MeshRenderer&& rhs) noexcept;

        const Guid& GetId() const override;

        GameObject& GetGameObject() const override;

        Bounds GetBounds() override;

        Transform& GetTransform() const override;

        Resource<MeshAsset> GetMesh() const override;
        void SetMesh(const Resource<MeshAsset>& value);

        Resource<MaterialAsset> GetMaterial() const override;
        void SetMaterial(const Resource<MaterialAsset>& value);
    };
}
