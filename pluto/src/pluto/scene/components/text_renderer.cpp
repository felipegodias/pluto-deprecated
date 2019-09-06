#include "pluto/scene/components/text_renderer.h"
#include "pluto/scene/components/component.impl.hpp"

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
    class TextRenderer::Impl : public Component::Impl
    {
        Resource<MeshAsset> mesh;

        std::string text;
        Resource<FontAsset> font;

        Anchor anchor;

        bool isDirty;

        MemoryManager* memoryManager;

    public:
        ~Impl()
        {
            memoryManager->Remove(*mesh.Get());
        }

        Impl(const Guid& guid, const Resource<GameObject>& gameObject, Resource<MeshAsset> mesh,
             MemoryManager& memoryManager)
            : Component::Impl(guid, gameObject),
              mesh(std::move(mesh)),
              anchor(Anchor::Default),
              isDirty(false),
              memoryManager(&memoryManager)
        {
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

        Anchor GetAnchor() const
        {
            return anchor;
        }

        void SetAnchor(const Anchor value)
        {
            anchor = value;
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
            float y = 0;
            float maxX = 0;

            uint32_t lineCount = 1;

            uint32_t t = 0;
            for (const char& c : text)
            {
                if (c == '\n')
                {
                    x = 0;
                    y -= fontAsset.Size();
                    ++lineCount;
                    continue;
                }

                if (c == '\t')
                {
                    x += fontAsset.GetGlyph(' ').advance * 4;
                    continue;
                }

                char character = c;
                if (!fontAsset.HasCharacter(character))
                {
                    character = '?';
                }

                const FontAsset::Glyph& glyph = fontAsset.GetGlyph(character);

                const float w = glyph.xMax - glyph.xMin;
                const float h = glyph.yMax - glyph.yMin;

                const float xPos = x + glyph.xBearing;
                const float yPos = -(h - abs(glyph.yBearing)) + y;

                Vector3F posA = {xPos, yPos, 0};
                Vector3F posB = {xPos + w, yPos, 0};
                Vector3F posC = {xPos, yPos + h, 0};
                Vector3F posD = {xPos + w, yPos + h, 0};

                positions.push_back(posA);
                positions.push_back(posB);
                positions.push_back(posC);
                positions.push_back(posD);

                uvs.emplace_back(glyph.xMin / 512, (512 - glyph.yMax) / 512);
                uvs.emplace_back(glyph.xMax / 512, (512 - glyph.yMax) / 512);
                uvs.emplace_back(glyph.xMin / 512, (512 - glyph.yMin) / 512);
                uvs.emplace_back(glyph.xMax / 512, (512 - glyph.yMin) / 512);

                triangles.emplace_back(t, t + 3, t + 1);
                triangles.emplace_back(t + 3, t, t + 2);

                t += 4;

                x += glyph.advance;
                maxX = std::max(x, maxX);
            }

            Vector3F offset = GetAnchorOffset(maxX, lineCount);

            for (auto& position : positions)
            {
                position = (position + offset) / 100;
            }

            mesh->SetPositions(positions);
            mesh->SetUVs(uvs);
            mesh->SetTriangles(triangles);
        }

        Vector3F GetAnchorOffset(const float maxX, const uint32_t lineCount)
        {
            const float size = font->Size();
            const float height = size * (static_cast<float>(lineCount) - 1.0f);

            // Magic PI??????
            const float pi = 3.14159265358979f;
            switch (anchor)
            {
            case Anchor::UpperLeft:
                return {0, -size, 0};
            case Anchor::UpperCenter:
                return {-maxX / 2, -size, 0};
            case Anchor::UpperRight:
                return {-maxX, -size, 0};
            case Anchor::MiddleLeft:
                return {0, -size / pi + height / 2, 0};
            case Anchor::MiddleCenter:
                return {-maxX / 2, -size / pi + height / 2, 0};
            case Anchor::MiddleRight:
                return {-maxX, -size / pi + height / 2, 0};
            case Anchor::LowerLeft:
                return {0, height, 0};
            case Anchor::LowerCenter:
                return {-maxX / 2, height, 0};
            case Anchor::LowerRight:
                return {-maxX, height, 0};
            }
            return Vector3F::ZERO;
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
        : Renderer(*impl),
          impl(std::move(impl))
    {
    }

    TextRenderer::TextRenderer(TextRenderer&& other) noexcept = default;

    TextRenderer& TextRenderer::operator=(TextRenderer&& rhs) noexcept = default;

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

    TextRenderer::Anchor TextRenderer::GetAnchor() const
    {
        return impl->GetAnchor();
    }

    void TextRenderer::SetAnchor(const Anchor value)
    {
        impl->SetAnchor(value);
    }

    void TextRenderer::OnUpdate()
    {
        impl->OnUpdate();
    }
}
