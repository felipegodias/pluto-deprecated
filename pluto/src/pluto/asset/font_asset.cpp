#include "pluto/asset/font_asset.h"

#include "pluto/guid.h"
#include "pluto/exception.h"
#include "pluto/asset/asset_manager.h"
#include "pluto/asset/material_asset.h"
#include "pluto/memory/resource.h"

#include "pluto/service/service_collection.h"

#include "pluto/file/reader.h"
#include "pluto/file/file_writer.h"

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
            fileWriter.Write(&Guid::PLUTO_IDENTIFIER, sizeof(Guid));

            uint8_t serializerVersion = 1;
            fileWriter.Write(&serializerVersion, sizeof(uint8_t));

            auto assetType = static_cast<uint8_t>(Type::Font);
            fileWriter.Write(&assetType, sizeof(uint8_t));

            fileWriter.Write(&guid, sizeof(Guid));

            uint8_t assetNameLength = name.size();
            fileWriter.Write(&assetNameLength, sizeof(uint8_t));
            fileWriter.Write(name.data(), assetNameLength);

            fileWriter.Write(&size, sizeof(float));

            uint8_t glyphsCount = glyphs.size();
            fileWriter.Write(&glyphsCount, sizeof(uint8_t));
            for (auto& it : glyphs)
            {
                const Glyph& glyph = it.second;
                fileWriter.Write(&glyph.character, sizeof(char));
                fileWriter.Write(&glyph.xMin, sizeof(float));
                fileWriter.Write(&glyph.yMin, sizeof(float));
                fileWriter.Write(&glyph.xMax, sizeof(float));
                fileWriter.Write(&glyph.yMax, sizeof(float));
                fileWriter.Write(&glyph.xBearing, sizeof(float));
                fileWriter.Write(&glyph.yBearing, sizeof(float));
                fileWriter.Write(&glyph.advance, sizeof(float));
            }

            Guid materialGuid = material.GetObjectId();
            fileWriter.Write(&materialGuid, sizeof(Guid));
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
                Exception::Throw(std::runtime_error(fmt::format("Glyph for '{0}' not found in {1} font asset.", character, name)));
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

    std::unique_ptr<Asset> FontAsset::Factory::Create(Reader& reader) const
    {
        Guid signature;
        reader.Read(&signature, sizeof(Guid));

        if (signature != Guid::PLUTO_IDENTIFIER)
        {
            Exception::Throw(
                std::runtime_error("Trying to load a asset but file signature does not match with pluto."));
        }

        uint8_t serializerVersion;
        reader.Read(&serializerVersion, sizeof(uint8_t));
        uint8_t assetType;
        reader.Read(&assetType, sizeof(uint8_t));

        if (assetType != static_cast<uint8_t>(Type::Font))
        {
            Exception::Throw(
                std::runtime_error("Trying to load a font but file is not a font asset."));
        }

        Guid assetId;
        reader.Read(&assetId, sizeof(Guid));
        uint8_t assetNameLength;
        reader.Read(&assetNameLength, sizeof(uint8_t));
        std::string assetName(assetNameLength, ' ');
        reader.Read(assetName.data(), assetNameLength);

        // Font asset from here!

        float size;
        reader.Read(&size, sizeof(float));

        uint8_t glyphsCount;
        reader.Read(&glyphsCount, sizeof(uint8_t));
        std::unordered_map<char, Glyph> glyphsMap;
        for (uint8_t i = 0; i < glyphsCount; ++i)
        {
            char character;
            reader.Read(&character, sizeof(char));
            float xMin;
            reader.Read(&xMin, sizeof(float));
            float yMin;
            reader.Read(&yMin, sizeof(float));
            float xMax;
            reader.Read(&xMax, sizeof(float));
            float yMax;
            reader.Read(&yMax, sizeof(float));
            float xBearing;
            reader.Read(&xBearing, sizeof(float));
            float yBearing;
            reader.Read(&yBearing, sizeof(float));
            float advance;
            reader.Read(&advance, sizeof(float));
            glyphsMap.emplace(character, Glyph{character, xMin, yMin, xMax, yMax, xBearing, yBearing, advance});
        }

        Guid materialGuid;
        reader.Read(&materialGuid, sizeof(Guid));
        ServiceCollection& serviceCollection = GetServiceCollection();
        auto& assetManager = serviceCollection.GetService<AssetManager>();
        Resource<MaterialAsset> material = assetManager.Load<MaterialAsset>(materialGuid);

        auto fontAsset = std::make_unique<FontAsset>(std::make_unique<Impl>(assetId, size, glyphsMap, material));

        return fontAsset;
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
