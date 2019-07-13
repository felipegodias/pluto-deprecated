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
        Impl(Guid guid, GameObject& gameObject) : guid(std::move(guid)), gameObject(gameObject), meshAsset(nullptr),
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

    MeshRenderer::Factory::Factory(DiContainer& diContainer) : BaseFactory(diContainer)
    {
    }

    std::unique_ptr<MeshRenderer> MeshRenderer::Factory::Create(GameObject& gameObject) const
    {
        return std::make_unique<MeshRenderer>(std::make_unique<Impl>(Guid::New(), gameObject));
    }

    MeshRenderer::MeshRenderer(std::unique_ptr<Impl> impl) : impl(std::move(impl))
    {
    }

    MeshRenderer::MeshRenderer(MeshRenderer&& other) noexcept : impl(std::move(other.impl))
    {
    }

    MeshRenderer::~MeshRenderer() = default;

    MeshRenderer& MeshRenderer::operator=(MeshRenderer&& rhs) noexcept
    {
        if (this == &rhs)
        {
            return *this;
        }

        impl = std::move(rhs.impl);
        return *this;
    }

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
