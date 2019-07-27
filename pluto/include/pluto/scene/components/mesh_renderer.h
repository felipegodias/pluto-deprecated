#pragma once

#include "pluto/scene/components/renderer.h"
#include "pluto/service/base_factory.h"

#include <memory>

namespace pluto
{
    class PLUTO_API MeshRenderer final : public Renderer
    {
    public:
        class PLUTO_API Factory final : public BaseFactory
        {
        public:
            explicit Factory(ServiceCollection& diContainer);
            std::unique_ptr<MeshRenderer> Create(GameObject& gameObject) const;
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

        MeshAsset& GetMesh() const override;
        void SetMesh(MeshAsset& value);

        MaterialAsset& GetMaterial() const override;
        void SetMaterial(MaterialAsset& value);
    };
}
