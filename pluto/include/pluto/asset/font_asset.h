#pragma once

#include "asset.h"
#include "pluto/service/base_factory.h"

#include <vector>

namespace pluto
{
    template <typename T, typename Enable = void>
    class Resource;

    class MaterialAsset;

    class PLUTO_API FontAsset final : public Asset
    {
    public:
        struct Glyph
        {
            char character;
            float xMin;
            float yMin;
            float xMax;
            float yMax;

            float xBearing;
            float yBearing;
            float advance;
        };

        class PLUTO_API Factory final : public Asset::Factory
        {
        public:
            explicit Factory(ServiceCollection& serviceCollection);
            std::unique_ptr<FontAsset> Create(float fontSize, const std::vector<Glyph>& glyphs,
                                              Resource<MaterialAsset>& material) const;

            std::unique_ptr<Asset> Create(StreamReader& reader) const override;
        };

    private:
        class Impl;
        std::unique_ptr<Impl> impl;

    public:
        ~FontAsset() override;

        explicit FontAsset(std::unique_ptr<Impl> impl);

        FontAsset(const FontAsset& other) = delete;
        FontAsset(FontAsset&& other) noexcept;
        FontAsset& operator=(const FontAsset& rhs) = delete;
        FontAsset& operator=(FontAsset&& rhs) noexcept;

        const Guid& GetId() const override;
        const std::string& GetName() const override;
        void SetName(const std::string& value) override;
        void Dump(FileStreamWriter& fileWriter) const override;

        float Size() const;

        bool HasCharacter(char character) const;
        const Glyph& GetGlyph(char character) const;

        Resource<MaterialAsset> GetMaterial() const;
    };
}
