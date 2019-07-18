#include "pluto/asset/texture_asset.h"

#include "pluto/math/color.h"
#include "pluto/math/vector2i.h"

#include "pluto/guid.h"

#include <utility>
#include <vector>

namespace pluto
{
    class TextureAsset::Impl
    {
        Guid guid;
        std::string name;
        Vector2I size;
        Format format;
        Filter filter;

        std::vector<uint8_t> pixels;

    public:
        explicit Impl(const Guid& guid) : guid(guid), format(Format::RGBA32), filter(Filter::Bilinear)
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

        const Vector2I& GetSize() const
        {
            return size;
        }

        void SetSize(const Vector2I& value)
        {
            size = value;
        }

        Color GetPixel(const Vector2I& pos) const
        {
            const uint32_t index = (pos.y * size.x + size.x) * GetChannelsCount();

            switch (format)
            {
            case Format::Alpha8:
                return {0, 0, 0, pixels[index]};
            case Format::RGB24:
                return {pixels[index], pixels[index + 1], pixels[index + 2], 0};
            case Format::RGBA32:
                return {pixels[index], pixels[index + 1], pixels[index + 2], pixels[index + 3],};
            }
            return Color::CLEAR;
        }

        void SetPixel(const Vector2I& pos, const Color& value)
        {
            const uint32_t index = (pos.y * size.x + size.x) * GetChannelsCount();
            switch (format)
            {
            case Format::Alpha8:
                pixels[index] = value.a;
                break;
            case Format::RGB24:
                pixels[index] = value.r;
                pixels[index + 1] = value.r;
                pixels[index + 2] = value.r;
                break;
            case Format::RGBA32:
                pixels[index] = value.r;
                pixels[index + 1] = value.g;
                pixels[index + 2] = value.b;
                pixels[index + 3] = value.a;
                break;
            }
        }

        std::vector<Color> GetPixels() const
        {
            std::vector<Color> buffer(pixels.size() / GetChannelsCount());
            switch (format)
            {
            case Format::Alpha8:
                for (size_t i = 0; i < pixels.size(); ++i)
                {
                    buffer[i].a = pixels[i];
                }
                break;
            case Format::RGB24:
                for (size_t i = 0, j = 0; i < pixels.size(); i += 3, ++j)
                {
                    buffer[j].r = pixels[i];
                    buffer[j].g = pixels[i + 1];
                    buffer[j].b = pixels[i + 2];
                }
                break;
            case Format::RGBA32:
                std::memcpy(buffer.data(), pixels.data(), pixels.size());
                break;
            }
            return buffer;
        }

        void SetPixels(const std::vector<Color>& value)
        {
            switch (format)
            {
            case Format::Alpha8:
                for (size_t i = 0; i < pixels.size() && i < value.size(); ++i)
                {
                    pixels[i] = value[i].a;
                }
                break;
            case Format::RGB24:
                for (size_t i = 0, j = 0; i < pixels.size() && j < value.size(); i += 3, ++j)
                {
                    pixels[i] = value[j].r;
                    pixels[i + 1] = value[j].g;
                    pixels[i + 2] = value[j].b;
                }
                break;
            case Format::RGBA32:
                std::memcpy(pixels.data(), value.data(), std::min(pixels.size(), value.size() * 4));
                break;
            }
        }

        Format GetFormat() const
        {
            return format;
        }

        void SetFormat(const Format value)
        {
            format = value;
        }

        Filter GetFilter() const
        {
            return filter;
        }

        void SetFilter(const Filter value)
        {
            filter = value;
        }

        void Clone(const Impl& other)
        {
            name = other.name;
            size = other.size;
            pixels = other.pixels;
            format = other.format;
            filter = other.filter;
        }

    private:
        uint8_t GetChannelsCount() const
        {
            switch (format)
            {
            case Format::Alpha8:
                return 1;
            case Format::RGB24:
                return 3;
            case Format::RGBA32:
                return 4;
            }
            return 0;
        }
    };

    TextureAsset::Factory::Factory(DiContainer& diContainer) : BaseFactory(diContainer)
    {
    }

    std::unique_ptr<TextureAsset> TextureAsset::Factory::Create() const
    {
        auto textureAsset = std::make_unique<TextureAsset>(std::make_unique<Impl>(Guid::New()));
        return textureAsset;
    }

    std::unique_ptr<TextureAsset> TextureAsset::Factory::Create(const TextureAsset& original) const
    {
        auto textureAsset = Create();
        textureAsset->Clone(original);
        return textureAsset;
    }

    std::unique_ptr<TextureAsset> TextureAsset::Factory::Create(FileReader& fileReader) const
    {
        return Create();
    }

    TextureAsset::TextureAsset(std::unique_ptr<Impl> impl) : impl(std::move(impl))
    {
    }

    TextureAsset::TextureAsset(TextureAsset&& other) noexcept = default;

    TextureAsset::~TextureAsset() = default;

    TextureAsset& TextureAsset::operator=(TextureAsset&& rhs) noexcept = default;

    const Guid& TextureAsset::GetId() const
    {
        return impl->GetId();
    }

    const std::string& TextureAsset::GetName() const
    {
        return impl->GetName();
    }

    void TextureAsset::SetName(const std::string value)
    {
        impl->SetName(value);
    }

    void TextureAsset::Dump(FileWriter& fileWriter) const
    {
        impl->Dump(fileWriter);
    }

    const Vector2I& TextureAsset::GetSize() const
    {
        return impl->GetSize();
    }

    void TextureAsset::SetSize(const Vector2I& value)
    {
        impl->SetSize(value);
    }

    Color TextureAsset::GetPixel(const Vector2I& pos) const
    {
        return impl->GetPixel(pos);
    }

    void TextureAsset::SetPixel(const Vector2I& pos, const Color& value)
    {
        impl->SetPixel(pos, value);
    }

    std::vector<Color> TextureAsset::GetPixels() const
    {
        return impl->GetPixels();
    }

    void TextureAsset::SetPixels(const std::vector<Color>& value) const
    {
        impl->SetPixels(value);
    }

    TextureAsset::Format TextureAsset::GetFormat() const
    {
        return impl->GetFormat();
    }

    void TextureAsset::SetFormat(const Format value)
    {
        impl->SetFormat(value);
    }

    TextureAsset::Filter TextureAsset::GetFilter() const
    {
        return impl->GetFilter();
    }

    void TextureAsset::SetFilter(const Filter value)
    {
        impl->SetFilter(value);
    }

    void TextureAsset::Clone(const TextureAsset& other)
    {
        impl->Clone(*other.impl);
    }
}
