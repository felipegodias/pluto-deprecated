#include "pluto/scene/components/mesh_renderer.h"
#include "pluto/scene/game_object.h"

#include "pluto/asset/mesh_asset.h"
#include "pluto/asset/material_asset.h"
#include "pluto/memory/resource.h"

#include "pluto/math/bounds.h"
#include "pluto/guid.h"

namespace pluto
{
    class MeshRenderer::Impl
    {
        Guid guid;
        GameObject& gameObject;

        Resource<MeshAsset> meshAsset;
        Resource<MaterialAsset> materialAsset;

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

        Resource<MeshAsset> GetMesh() const
        {
            return meshAsset;
        }

        void SetMesh(const Resource<MeshAsset>& value)
        {
            meshAsset = value;
        }

        Resource<MaterialAsset> GetMaterial() const
        {
            return materialAsset;
        }

        void SetMaterial(const Resource<MaterialAsset>& value)
        {
            materialAsset = value;
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

    Resource<MeshAsset> MeshRenderer::GetMesh() const
    {
        return impl->GetMesh();
    }

    void MeshRenderer::SetMesh(const Resource<MeshAsset>& value)
    {
        impl->SetMesh(value);
    }

    Resource<MaterialAsset> MeshRenderer::GetMaterial() const
    {
        return impl->GetMaterial();
    }

    void MeshRenderer::SetMaterial(const Resource<MaterialAsset>& value)
    {
        impl->SetMaterial(value);
    }
}
