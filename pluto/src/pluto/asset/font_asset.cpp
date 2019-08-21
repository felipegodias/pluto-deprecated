#include "pluto/asset/font_asset.h"

#include "pluto/guid.h"
#include "pluto/exception.h"
#include "pluto/asset/material_asset.h"
#include "pluto/memory/resource.h"

#include <fmt/format.h>
#include <unordered_map>
#include <utility>

namespace pluto
{
    class FontAsset::Impl
    {
        Guid guid;
        std::string name;
        float size;

        std::unordered_map<char, Glyph> glyphs;
        Resource<MaterialAsset> material;

    public:
        Impl(const Guid& guid, const float size, std::unordered_map<char, Glyph> glyphs,
             Resource<MaterialAsset> material)
            : guid(guid),
              size(size),
              glyphs(std::move(glyphs)),
              material(std::move(material))
        {
        }

        const Guid& GetId() const
        {
            return guid;
        }

        const std::string& GetName() const
        {
            return name;
        }

        void SetName(const std::string& value)
        {
            name = value;
        }

        void Dump(FileWriter& fileWriter) const
        {
        }

        float Size() const
        {
            return size;
        }

        bool HasCharacter(const char character) const
        {
            return glyphs.find(character) != glyphs.end();
        }

        const Glyph& GetGlyph(const char character) const
        {
            const auto it = glyphs.find(character);
            if (it == glyphs.end())
            {
                Exception::Throw(std::runtime_error(fmt::format("Glyph {0} not found in {1} font asset.")));
            }
            return it->second;
        }

        Resource<MaterialAsset> GetMaterial() const
        {
            return material;
        }
    };

    FontAsset::Factory::Factory(ServiceCollection& serviceCollection)
        : Asset::Factory(serviceCollection)
    {
    }

    std::unique_ptr<FontAsset> FontAsset::Factory::Create(float fontSize, const std::vector<Glyph>& glyphs,
                                                          Resource<MaterialAsset>& material) const
    {
        std::unordered_map<char, Glyph> glyphsMap;
        for (auto& glyph : glyphs)
        {
            glyphsMap.emplace(glyph.character, glyph);
        }
        return std::make_unique<FontAsset>(std::make_unique<Impl>(Guid(), fontSize, glyphsMap, material));
    }

    std::unique_ptr<Asset> FontAsset::Factory::Create(FileReader& fileReader) const
    {
        return nullptr; // std::make_unique<FontAsset>(std::make_unique<Impl>());
    }

    FontAsset::~FontAsset() = default;

    FontAsset::FontAsset(std::unique_ptr<Impl> impl)
        : impl(std::move(impl))
    {
    }

    FontAsset::FontAsset(FontAsset&& other) noexcept = default;

    FontAsset& FontAsset::operator=(FontAsset&& rhs) noexcept = default;

    const Guid& FontAsset::GetId() const
    {
        return impl->GetId();
    }

    const std::string& FontAsset::GetName() const
    {
        return impl->GetName();
    }

    void FontAsset::SetName(const std::string& value)
    {
        return impl->SetName(value);
    }

    void FontAsset::Dump(FileWriter& fileWriter) const
    {
        impl->Dump(fileWriter);
    }

    float FontAsset::Size() const
    {
        return impl->Size();
    }

    bool FontAsset::HasCharacter(const char character) const
    {
        return impl->HasCharacter(character);
    }

    const FontAsset::Glyph& FontAsset::GetGlyph(const char character) const
    {
        return impl->GetGlyph(character);
    }

    Resource<MaterialAsset> FontAsset::GetMaterial() const
    {
        return impl->GetMaterial();
    }
}
