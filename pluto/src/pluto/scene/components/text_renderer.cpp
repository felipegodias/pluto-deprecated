#include "pluto/scene/components/text_renderer.h"

#include "pluto/memory/memory_manager.h"
#include "pluto/memory/resource.h"

#include "pluto/service/service_collection.h"

#include "pluto/scene/game_object.h"
#include "pluto/asset/font_asset.h"
#include "pluto/asset/material_asset.h"
#include "pluto/asset/mesh_asset.h"

#include "pluto/math/bounds.h"
#include "pluto/math/vector3f.h"
#include "pluto/math/vector2f.h"
#include "pluto/math/vector3i.h"

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

        bool isDirty;

        MemoryManager* memoryManager;

    public:
        ~Impl()
        {
            memoryManager->Remove(*mesh.Get());
        }

        Impl(const Guid& guid, Resource<GameObject> gameObject, Resource<MeshAsset> mesh, MemoryManager& memoryManager)
            : guid(guid),
              gameObject(std::move(gameObject)),
              mesh(std::move(mesh)),
              memoryManager(&memoryManager)
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
            isDirty = true;
        }

        Resource<FontAsset> GetFont() const
        {
            return font;
        }

        void SetFont(const Resource<FontAsset>& value)
        {
            font = value;
            isDirty = true;
        }

        void OnUpdate()
        {
            if (!isDirty)
            {
                return;
            }

            UpdateMesh();
            isDirty = false;
        }

    private:
        void UpdateMesh()
        {
            std::vector<Vector3F> positions;
            std::vector<Vector2F> uvs;
            std::vector<Vector3I> triangles;

            FontAsset& fontAsset = *font.Get();
            float x = 0; 
            uint32_t t = 0;
            for (const char& c : text)
            {
                const FontAsset::Glyph& glyph = fontAsset.GetGlyph(c);

                const float w = glyph.xMax - glyph.xMin;
                const float h = glyph.yMax - glyph.yMin;

                float xPos = x + glyph.xBearing;
                float yPos = -(h - abs(glyph.yBearing));

                Vector3F posA = { xPos, yPos, 0 };
                Vector3F posB = { xPos + w, yPos, 0 };
                Vector3F posC = { xPos, yPos + h, 0 };
                Vector3F posD = { xPos + w, yPos + h, 0 };

                positions.push_back(posA / 100);
                positions.push_back(posB / 100);
                positions.push_back(posC / 100);
                positions.push_back(posD / 100);

                uvs.emplace_back(glyph.xMin / 512, (512 - glyph.yMax) / 512);
                uvs.emplace_back(glyph.xMax / 512, (512 - glyph.yMax) / 512);
                uvs.emplace_back(glyph.xMin / 512, (512 - glyph.yMin) / 512);
                uvs.emplace_back(glyph.xMax / 512, (512 - glyph.yMin) / 512);

                triangles.emplace_back(t, t + 3, t + 1);
                triangles.emplace_back(t + 3, t, t + 2);

                t += 4;
                x += glyph.advance;
            }

            mesh->SetPositions(positions);
            mesh->SetUVs(uvs);
            mesh->SetTriangles(triangles);
        }
    };

    TextRenderer::Factory::Factory(ServiceCollection& serviceCollection)
        : Component::Factory(serviceCollection)
    {
    }

    std::unique_ptr<Component> TextRenderer::Factory::Create(const Resource<GameObject>& gameObject) const
    {
        ServiceCollection& serviceCollection = GetServiceCollection();
        MeshAsset::Factory& meshAssetFactory = serviceCollection.GetFactory<MeshAsset>();
        std::unique_ptr<MeshAsset> meshAsset = meshAssetFactory.Create();

        auto& memoryManager = serviceCollection.GetService<MemoryManager>();
        Resource<MeshAsset> meshAssetResource = ResourceUtils::Cast<MeshAsset>(memoryManager.Add(std::move(meshAsset)));

        return std::make_unique<TextRenderer>(
            std::make_unique<Impl>(Guid::New(), gameObject, meshAssetResource, memoryManager));
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

    void TextRenderer::OnUpdate()
    {
        impl->OnUpdate();
    }
}
