#include "pluto/scene/components/mesh_renderer.h"
#include "pluto/scene/game_object.h"

#include "pluto/math/bounds.h"
#include "pluto/guid.h"

namespace pluto
{
    class MeshRenderer::Impl
    {
        Guid guid;
        GameObject& gameObject;

        MeshAsset* meshAsset;
        MaterialAsset* materialAsset;

    public:
        Impl(const Guid& guid, GameObject& gameObject)
            : guid(guid),
              gameObject(gameObject),
              meshAsset(nullptr),
              materialAsset(nullptr)
        {
        }

        const Guid& GetId() const
        {
            return guid;
        }

        GameObject& GetGameObject() const
        {
            return gameObject;
        }

        Bounds GetBounds()
        {
            return Bounds();
        }

        Transform& GetTransform() const
        {
            return gameObject.GetTransform();
        }

        MeshAsset& GetMesh() const
        {
            return *meshAsset;
        }

        void SetMesh(MeshAsset& value)
        {
            meshAsset = &value;
        }

        MaterialAsset& GetMaterial() const
        {
            return *materialAsset;
        }

        void SetMaterial(MaterialAsset& value)
        {
            materialAsset = &value;
        }
    };

    MeshRenderer::Factory::~Factory() = default;

    MeshRenderer::Factory::Factory(ServiceCollection& diContainer)
        : Component::Factory(diContainer)
    {
    }

    MeshRenderer::Factory::Factory(Factory&& other) noexcept = default;

    MeshRenderer::Factory& MeshRenderer::Factory::operator=(Factory&& rhs) noexcept = default;

    std::unique_ptr<Component> MeshRenderer::Factory::Create(GameObject& gameObject) const
    {
        return std::make_unique<MeshRenderer>(std::make_unique<Impl>(Guid::New(), gameObject));
    }

    MeshRenderer::~MeshRenderer() = default;

    MeshRenderer::MeshRenderer(std::unique_ptr<Impl> impl)
        : impl(std::move(impl))
    {
    }

    MeshRenderer::MeshRenderer(MeshRenderer&& other) noexcept = default;

    MeshRenderer& MeshRenderer::operator=(MeshRenderer&& rhs) noexcept = default;

    const Guid& MeshRenderer::GetId() const
    {
        return impl->GetId();
    }

    GameObject& MeshRenderer::GetGameObject() const
    {
        return impl->GetGameObject();
    }

    Bounds MeshRenderer::GetBounds()
    {
        return impl->GetBounds();
    }

    Transform& MeshRenderer::GetTransform() const
    {
        return impl->GetTransform();
    }

    MeshAsset& MeshRenderer::GetMesh() const
    {
        return impl->GetMesh();
    }

    void MeshRenderer::SetMesh(MeshAsset& value)
    {
        impl->SetMesh(value);
    }

    MaterialAsset& MeshRenderer::GetMaterial() const
    {
        return impl->GetMaterial();
    }

    void MeshRenderer::SetMaterial(MaterialAsset& value)
    {
        impl->SetMaterial(value);
    }
}
