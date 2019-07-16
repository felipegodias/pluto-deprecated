#pragma once

#include "asset.h"
#include "../di/base_factory.h"
#include <memory>
#include <vector>

namespace pluto
{
    class Vector2I;
    class Vector4F;
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
     */
    class PLUTO_API TextureAsset final : public Asset
    {
    public:
        enum class Format
        {
            Alpha8 = 0,
            RGB24 = 1,
            RGBA32 = 2
        };

        enum class Filter
        {
            Point = 0,
            Bilinear = 1
        };

        class PLUTO_API Factory final : public BaseFactory
        {
        public:
            explicit Factory(DiContainer& diContainer);
            std::unique_ptr<TextureAsset> Create() const;
            std::unique_ptr<TextureAsset> Create(const TextureAsset& original) const;
            std::unique_ptr<TextureAsset> Create(FileReader& fileReader) const;
        };

    private:
        class Impl;
        std::unique_ptr<Impl> impl;

    public:
        explicit TextureAsset(std::unique_ptr<Impl> impl);
        TextureAsset(const TextureAsset& other) = delete;
        TextureAsset(TextureAsset&& other) noexcept;
        ~TextureAsset() override;

        TextureAsset& operator=(const TextureAsset& rhs);
        TextureAsset& operator=(TextureAsset&& rhs) noexcept;

        const Guid& GetId() const override;
        const std::string& GetName() const override;
        void SetName(std::string value) override;
        void Dump(FileWriter& fileWriter) const override;

        const Vector2I& GetSize() const;
        void SetSize(Vector2I value);

        Vector4F GetPixel(const Vector2I& pos) const;
        void SetPixel(const Vector2I& pos, const Vector4F& value);

        std::vector<Vector4F> GetPixels() const;
        void SetPixels(const std::vector<Vector4F>& value) const;

        Format GetFormat() const;
        void SetFormat(Format value);

        Filter GetFilter() const;
        void SetFilter(Filter value);
    };
}
