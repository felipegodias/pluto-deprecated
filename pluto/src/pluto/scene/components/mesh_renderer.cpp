#include "pluto/scene/components/mesh_renderer.h"
#include "pluto/scene/components/component.impl.hpp"
#include "pluto/scene/game_object.h"

#include "pluto/asset/mesh_asset.h"
#include "pluto/asset/material_asset.h"
#include "pluto/memory/resource.h"

#include "pluto/math/bounds.h"
#include "pluto/guid.h"

namespace pluto
{
    class MeshRenderer::Impl : public Component::Impl
    {
        Resource<MeshAsset> meshAsset;
        Resource<MaterialAsset> materialAsset;

    public:
        Impl(const Guid& guid, const Resource<GameObject>& gameObject)
            : Component::Impl(guid, gameObject),
              meshAsset(nullptr),
              materialAsset(nullptr)
        {
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
        : Renderer(*impl),
          impl(std::move(impl))
    {
    }

    MeshRenderer::MeshRenderer(MeshRenderer&& other) noexcept = default;

    MeshRenderer& MeshRenderer::operator=(MeshRenderer&& rhs) noexcept = default;

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
