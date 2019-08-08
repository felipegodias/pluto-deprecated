#pragma once

#include "asset.h"
#include "pluto/service/base_factory.h"

#include <memory>
#include <vector>

namespace pluto
{
    template <typename T, typename Enable = void>
    class Resource;

    class Rect;
    class Color;
    class TextureBuffer;
    class FileReader;

    /*
     * File layout in disk. (Version 1)
     * +--------------+------+------------------------------+
     * | Type         | Size | Description                  |
     * +--------------+------+------------------------------+
     * | GUID         | 16   | File signature.              |
     * | uint8_t      | 1    | Serializer version.          |
     * | uint8_t      | 1    | Asset type.                  |
     * | GUID         | 16   | Asset unique identifier.     |
     * | uint8_t      | 1    | Asset name length.           |
     * | string       | *    | Asset name.                  |
     * +--------------+------+------------------------------+
     * | uint8_t      | 1    | Filter.                      |
     * | uint8_t      | 1    | Format.                      |
     * | uint32_t     | 4    | Bytes count.                 |
     * | uint8_t[]    | *    | Bytes.                       |
     * +--------------+------+------------------------------+
     */
    class PLUTO_API TextureAsset final : public Asset
    {
    public:
        enum class Wrap
        {
            Clamp = 0,
            Mirror = 1,
            Repeat = 2,
            Default = Clamp,
            Last = Repeat,
            Count = Last + 1
        };

        enum class Format
        {
            Alpha8 = 0,
            RGB24 = 1,
            RGBA32 = 2,
            Default = RGBA32,
            Last = RGBA32,
            Count = Last + 1
        };

        enum class Filter
        {
            Point = 0,
            Bilinear = 1,
            Default = Bilinear,
            Last = Bilinear,
            Count = Last + 1
        };

        class PLUTO_API Factory final : public Asset::Factory
        {
        public:
            explicit Factory(ServiceCollection& serviceCollection);

            std::unique_ptr<TextureAsset> Create(uint16_t width, uint16_t height) const;
            std::unique_ptr<TextureAsset> Create(uint16_t width, uint16_t height, Format format) const;
            std::unique_ptr<TextureAsset> Create(uint16_t width, uint16_t height, Format format,
                                                 std::vector<uint8_t> data) const;
            std::unique_ptr<TextureAsset> Create(const TextureAsset& original) const;
            std::unique_ptr<Asset> Create(FileReader& fileReader) const override;
        };

    private:
        class Impl;
        std::unique_ptr<Impl> impl;

    public:
        ~TextureAsset() override;
        explicit TextureAsset(std::unique_ptr<Impl> impl);

        TextureAsset(const TextureAsset& other) = delete;
        TextureAsset(TextureAsset&& other) noexcept;
        TextureAsset& operator=(const TextureAsset& rhs) = delete;
        TextureAsset& operator=(TextureAsset&& rhs) noexcept;

        const Guid& GetId() const override;
        const std::string& GetName() const override;
        void SetName(const std::string& value) override;

        void Dump(FileWriter& fileWriter) const override;

        std::vector<uint8_t> Data();

        uint16_t GetWidth() const;
        uint16_t GetHeight() const;

        Color GetPixel(uint16_t x, uint16_t y) const;
        void SetPixel(uint16_t x, uint16_t y, const Color& value);

        std::vector<Color> GetPixels() const;
        void SetPixels(const std::vector<Color>& value) const;

        Wrap GetWrap() const;
        void SetWrap(Wrap value);

        Format GetFormat() const;

        Filter GetFilter() const;
        void SetFilter(Filter value);

        TextureBuffer& GetTextureBuffer();

        std::vector<Rect> PackTextures(const std::vector<Resource<TextureAsset>>& textures, uint8_t padding);

        void Apply();
        void Clone(const TextureAsset& other);
    };
}
