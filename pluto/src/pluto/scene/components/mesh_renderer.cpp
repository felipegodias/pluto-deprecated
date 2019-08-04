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
        Resource<GameObject> gameObject;

        Resource<MeshAsset> meshAsset;
        Resource<MaterialAsset> materialAsset;

    public:
        Impl(const Guid& guid, Resource<GameObject> gameObject)
            : guid(guid),
              gameObject(std::move(gameObject)),
              meshAsset(nullptr),
              materialAsset(nullptr)
        {
        }

        const Guid& GetId() const
        {
            return guid;
        }

        const std::string& GetName() const
        {
            return gameObject->GetName();
        }

        void SetName(const std::string& value)
        {
            gameObject->SetName(value);
        }

        Resource<GameObject> GetGameObject() const
        {
            return gameObject;
        }

        Bounds GetBounds()
        {
            return Bounds();
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

    MeshRenderer::Factory::Factory(ServiceCollection& serviceCollection)
        : Component::Factory(serviceCollection)
    {
    }

    std::unique_ptr<Component> MeshRenderer::Factory::Create(const Resource<GameObject>& gameObject) const
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

    const std::string& MeshRenderer::GetName() const
    {
        return impl->GetName();
    }

    void MeshRenderer::SetName(const std::string& value)
    {
        impl->SetName(value);
    }

    Resource<GameObject> MeshRenderer::GetGameObject() const
    {
        return impl->GetGameObject();
    }

    Bounds MeshRenderer::GetBounds()
    {
        return impl->GetBounds();
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
