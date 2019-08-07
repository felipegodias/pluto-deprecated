#include "pluto/asset/texture_asset.h"

#include "pluto/render/texture_buffer.h"

#include "pluto/service/service_collection.h"

#include "pluto/file/file_writer.h"
#include "pluto/file/file_reader.h"

#include "pluto/math/color.h"
#include "pluto/math/vector2i.h"

#include "pluto/guid.h"
#include "pluto/exception.h"

#include <memory>
#include <utility>
#include <vector>

namespace pluto
{
    class TextureAsset::Impl
    {
        Guid guid;
        std::string name;

        uint16_t width;
        uint16_t height;
        Format format;

        Wrap wrap;
        Filter filter;

        std::unique_ptr<TextureBuffer> textureBuffer;

        std::vector<uint8_t> data;

        TextureAsset* instance;

    public:
        Impl(const Guid& guid, const uint16_t width, const uint16_t height, const Format format,
             std::vector<uint8_t> data, std::unique_ptr<TextureBuffer> textureBuffer)
            : guid(guid),
              width(width),
              height(height),
              format(format),
              wrap(Wrap::Default),
              filter(Filter::Default),
              textureBuffer(std::move(textureBuffer)),
              data(std::move(data)),
              instance(nullptr)

        {
        }

        void Init(TextureAsset& textureAsset)
        {
            instance = &textureAsset;
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
            auto assetType = static_cast<uint8_t>(Type::Texture);
            fileWriter.Write(&assetType, sizeof(uint8_t));
            fileWriter.Write(&guid, sizeof(Guid));
            uint8_t assetNameLength = name.size();
            fileWriter.Write(&assetNameLength, sizeof(uint8_t));
            fileWriter.Write(name.data(), assetNameLength);

            fileWriter.Write(&width, sizeof(uint16_t));
            fileWriter.Write(&height, sizeof(uint16_t));

            auto format = static_cast<uint8_t>(this->format);
            fileWriter.Write(&format, sizeof(uint8_t));

            auto wrap = static_cast<uint8_t>(this->wrap);
            fileWriter.Write(&wrap, sizeof(uint8_t));

            auto filter = static_cast<uint8_t>(this->filter);
            fileWriter.Write(&filter, sizeof(uint8_t));

            uint32_t dataSize = data.size();
            fileWriter.Write(&dataSize, sizeof(uint32_t));
            fileWriter.Write(data.data(), dataSize);
        }

        std::vector<uint8_t> Data()
        {
            return data;
        }

        uint16_t GetWidth() const
        {
            return width;
        }

        uint16_t GetHeight() const
        {
            return height;
        }

        Color GetPixel(const uint16_t x, const uint16_t y) const
        {
            const size_t index = (y * static_cast<size_t>(width) + x) * GetChannelsCount();

            switch (format)
            {
            case Format::Alpha8:
                return {0, 0, 0, data[index]};
            case Format::RGB24:
                return {data[index], data[index + 1], data[index + 2], 0};
            case Format::RGBA32:
                return {data[index], data[index + 1], data[index + 2], data[index + 3],};
            default: ;
            }
            return Color::CLEAR;
        }

        void SetPixel(const uint16_t x, const uint16_t y, const Color& value)
        {
            const size_t index = (y * static_cast<size_t>(width) + x) * GetChannelsCount();
            switch (format)
            {
            case Format::Alpha8:
                data[index] = value.a;
                break;
            case Format::RGB24:
                data[index] = value.r;
                data[index + 1] = value.r;
                data[index + 2] = value.r;
                break;
            case Format::RGBA32:
                data[index] = value.r;
                data[index + 1] = value.g;
                data[index + 2] = value.b;
                data[index + 3] = value.a;
                break;
            default: ;
            }
        }

        std::vector<Color> GetPixels() const
        {
            // TODO: Check for possible division by zero when format is not true color.
            std::vector<Color> buffer(data.size() / GetChannelsCount());
            switch (format)
            {
            case Format::Alpha8:
                for (size_t i = 0; i < data.size(); ++i)
                {
                    buffer[i].a = data[i];
                }
                break;
            case Format::RGB24:
                for (size_t i = 0, j = 0; i < data.size(); i += 3, ++j)
                {
                    buffer[j].r = data[i];
                    buffer[j].g = data[i + 1];
                    buffer[j].b = data[i + 2];
                }
                break;
            case Format::RGBA32:
                std::memcpy(buffer.data(), data.data(), data.size());
                break;
            default: ;
            }
            return buffer;
        }

        void SetPixels(const std::vector<Color>& value)
        {
            // TODO: Check for value size, must be the same size as width times height.
            switch (format)
            {
            case Format::Alpha8:
                for (size_t i = 0; i < data.size() && i < value.size(); ++i)
                {
                    data[i] = value[i].a;
                }
                break;
            case Format::RGB24:
                for (size_t i = 0, j = 0; i < data.size() && j < value.size(); i += 3, ++j)
                {
                    data[i] = value[j].r;
                    data[i + 1] = value[j].g;
                    data[i + 2] = value[j].b;
                }
                break;
            case Format::RGBA32:
                std::memcpy(data.data(), value.data(), std::min(data.size(), value.size() * 4));
                break;
            default: ;
            }
        }

        Wrap GetWrap() const
        {
            return wrap;
        }

        void SetWrap(const Wrap value)
        {
            wrap = value;
        }

        Format GetFormat() const
        {
            return format;
        }

        Filter GetFilter() const
        {
            return filter;
        }

        void SetFilter(const Filter value)
        {
            filter = value;
        }

        TextureBuffer& GetTextureBuffer()
        {
            return *textureBuffer;
        }

        void Clone(const Impl& other)
        {
            // TODO: Validate size and formats.
            name = other.name;
            data = other.data;
            wrap = other.wrap;
            filter = other.filter;
        }

        void Apply()
        {
            textureBuffer->Update(*instance);
        }

    private:
        size_t GetChannelsCount() const
        {
            switch (format)
            {
            case Format::Alpha8:
                return 1;
            case Format::RGB24:
                return 3;
            case Format::RGBA32:
                return 4;
            default: ;
            }
            return 0;
        }
    };

    TextureAsset::Factory::Factory(ServiceCollection& serviceCollection)
        : Asset::Factory(serviceCollection)
    {
    }

    std::unique_ptr<TextureAsset> TextureAsset::Factory::Create(const uint16_t width, const uint16_t height) const
    {
        return Create(width, height, Format::Default);
    }

    std::unique_ptr<TextureAsset> TextureAsset::Factory::Create(const uint16_t width, const uint16_t height,
                                                                const Format format) const
    {
        uint8_t bpp = 0;
        switch (format)
        {
        case Format::Alpha8:
            bpp = 1;
            break;
        case Format::RGB24:
            bpp = 3;
            break;
        case Format::RGBA32:
            bpp = 4;
            break;
        default: ;
        }

        std::vector<uint8_t> data(static_cast<size_t>(width) * height * bpp);
        return Create(width, height, format, std::move(data));
    }

    std::unique_ptr<TextureAsset> TextureAsset::Factory::Create(uint16_t width, uint16_t height, Format format,
                                                                std::vector<uint8_t> data) const
    {
        ServiceCollection& serviceCollection = GetServiceCollection();
        const auto& textureBufferFactory = serviceCollection.GetFactory<TextureBuffer>();
        auto textureBuffer = textureBufferFactory.Create();

        auto textureAsset = std::make_unique<TextureAsset>(
            std::make_unique<Impl>(Guid::New(), width, height, format, std::move(data), std::move(textureBuffer)));

        textureAsset->impl->Init(*textureAsset);
        textureAsset->Apply();

        return textureAsset;
    }

    std::unique_ptr<TextureAsset> TextureAsset::Factory::Create(const TextureAsset& original) const
    {
        auto textureAsset = Create(original.GetWidth(), original.GetHeight(), original.GetFormat());
        textureAsset->Clone(original);
        textureAsset->Apply();
        return textureAsset;
    }

    std::unique_ptr<Asset> TextureAsset::Factory::Create(FileReader& fileReader) const
    {
        Guid signature;
        fileReader.Read(&signature, sizeof(Guid));

        if (signature != Guid::PLUTO_IDENTIFIER)
        {
            Exception::Throw(
                std::runtime_error("Trying to load a texture but file signature does not match with pluto."));
        }

        uint8_t serializerVersion;
        fileReader.Read(&serializerVersion, sizeof(uint8_t));
        uint8_t assetType;
        fileReader.Read(&assetType, sizeof(uint8_t));

        if (assetType != static_cast<uint8_t>(Type::Texture))
        {
            Exception::Throw(
                std::runtime_error("Trying to load a texture but file is not a texture asset."));
        }

        Guid assetId;
        fileReader.Read(&assetId, sizeof(Guid));
        uint8_t assetNameLength;
        fileReader.Read(&assetNameLength, sizeof(uint8_t));
        std::string assetName(assetNameLength, ' ');
        fileReader.Read(assetName.data(), assetNameLength);

        uint16_t width;
        fileReader.Read(&width, sizeof(uint16_t));

        uint16_t height;
        fileReader.Read(&height, sizeof(uint16_t));

        uint8_t format;
        fileReader.Read(&format, sizeof(uint8_t));

        uint8_t wrap;
        fileReader.Read(&wrap, sizeof(uint8_t));

        uint8_t filter;
        fileReader.Read(&filter, sizeof(uint8_t));

        uint32_t dataSize;
        fileReader.Read(&dataSize, sizeof(uint32_t));

        std::vector<uint8_t> data(dataSize);
        fileReader.Read(data.data(), dataSize);

        auto textureAsset = Create(width, height, static_cast<Format>(format), std::move(data));
        textureAsset->SetName(assetName);
        textureAsset->SetWrap(static_cast<Wrap>(wrap));
        textureAsset->SetFilter(static_cast<Filter>(filter));

        return textureAsset;
    }

    TextureAsset::TextureAsset(std::unique_ptr<Impl> impl)
        : impl(std::move(impl))
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

    void TextureAsset::SetName(const std::string& value)
    {
        impl->SetName(value);
    }

    void TextureAsset::Dump(FileWriter& fileWriter) const
    {
        impl->Dump(fileWriter);
    }

    std::vector<uint8_t> TextureAsset::Data()
    {
        return impl->Data();
    }

    uint16_t TextureAsset::GetWidth() const
    {
        return impl->GetWidth();
    }

    uint16_t TextureAsset::GetHeight() const
    {
        return impl->GetHeight();
    }

    Color TextureAsset::GetPixel(uint16_t x, uint16_t y) const
    {
        return impl->GetPixel(x, y);
    }

    void TextureAsset::SetPixel(uint16_t x, uint16_t y, const Color& value)
    {
        return impl->SetPixel(x, y, value);
    }

    std::vector<Color> TextureAsset::GetPixels() const
    {
        return impl->GetPixels();
    }

    void TextureAsset::SetPixels(const std::vector<Color>& value) const
    {
        impl->SetPixels(value);
    }

    TextureAsset::Wrap TextureAsset::GetWrap() const
    {
        return impl->GetWrap();
    }

    void TextureAsset::SetWrap(const Wrap value)
    {
        impl->SetWrap(value);
    }

    TextureAsset::Format TextureAsset::GetFormat() const
    {
        return impl->GetFormat();
    }

    TextureAsset::Filter TextureAsset::GetFilter() const
    {
        return impl->GetFilter();
    }

    void TextureAsset::SetFilter(const Filter value)
    {
        impl->SetFilter(value);
    }

    TextureBuffer& TextureAsset::GetTextureBuffer()
    {
        return impl->GetTextureBuffer();
    }

    void TextureAsset::Apply()
    {
        impl->Apply();
    }

    void TextureAsset::Clone(const TextureAsset& other)
    {
        impl->Clone(*other.impl);
    }
}
