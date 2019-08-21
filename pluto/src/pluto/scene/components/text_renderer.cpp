#include "pluto/scene/components/text_renderer.h"

#include "pluto/scene/game_object.h"
#include "pluto/asset/font_asset.h"
#include "pluto/asset/material_asset.h"
#include "pluto/asset/mesh_asset.h"

#include "pluto/math/bounds.h"

#include <string>

namespace pluto
{
    class TextRenderer::Impl
    {
        Guid guid;
        Resource<GameObject> gameObject;

        Resource<MeshAsset> mesh;

        std::string text;
        Resource<FontAsset> font;

    public:
        Impl(const Guid& guid, Resource<GameObject> gameObject, Resource<MeshAsset> mesh)
            : guid(guid),
              gameObject(std::move(gameObject)),
              mesh(std::move(mesh))
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
            return mesh;
        }

        Resource<MaterialAsset> GetMaterial() const
        {
            return font->GetMaterial();
        }

        const std::string& GetText() const
        {
            return text;
        }

        void SetText(const std::string& value)
        {
            text = value;
        }

        Resource<FontAsset> GetFont() const
        {
            return font;
        }

        void SetFont(const Resource<FontAsset>& value)
        {
            font = value;
        }
    };

    TextRenderer::Factory::Factory(ServiceCollection& serviceCollection)
        : Component::Factory(serviceCollection)
    {
    }

    std::unique_ptr<Component> TextRenderer::Factory::Create(const Resource<GameObject>& gameObject) const
    {
    }

    TextRenderer::~TextRenderer() = default;

    TextRenderer::TextRenderer(std::unique_ptr<Impl> impl)
        : impl(std::move(impl))
    {
    }

    TextRenderer::TextRenderer(TextRenderer&& other) noexcept = default;

    TextRenderer& TextRenderer::operator=(TextRenderer&& rhs) noexcept = default;

    const Guid& TextRenderer::GetId() const
    {
        return impl->GetId();
    }

    const std::string& TextRenderer::GetName() const
    {
        return impl->GetName();
    }

    void TextRenderer::SetName(const std::string& value)
    {
        impl->SetName(value);
    }

    Resource<GameObject> TextRenderer::GetGameObject() const
    {
        return impl->GetGameObject();
    }

    Bounds TextRenderer::GetBounds()
    {
        return impl->GetBounds();
    }

    Resource<MeshAsset> TextRenderer::GetMesh() const
    {
        return impl->GetMesh();
    }

    Resource<MaterialAsset> TextRenderer::GetMaterial() const
    {
        return impl->GetMaterial();
    }

    const std::string& TextRenderer::GetText() const
    {
        return impl->GetText();
    }

    void TextRenderer::SetText(const std::string& value)
    {
        impl->SetText(value);
    }

    Resource<FontAsset> TextRenderer::GetFont() const
    {
        return impl->GetFont();
    }

    void TextRenderer::SetFont(const Resource<FontAsset>& value)
    {
        impl->SetFont(value);
    }
}
