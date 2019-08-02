#include <pluto/asset/text_asset.h>
#include <pluto/file/file_reader.h>
#include <pluto/file/file_writer.h>
#include <pluto/guid.h>

#include <vector>

namespace pluto
{
    class TextAsset::Impl
    {
    private:
        Guid guid;
        std::string name;
        std::string text;

    public:
        explicit Impl(Guid guid)
            : guid(std::move(guid))
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

        void SetName(std::string value)
        {
            name = std::move(value);
        }

        void Dump(FileWriter& fileWriter) const
        {
            fileWriter.Write(&guid, sizeof(Guid));
            uint8_t serializerVersion = 1;
            fileWriter.Write(&serializerVersion, sizeof(uint8_t));
            uint8_t assetType = 1;
            fileWriter.Write(&assetType, sizeof(uint8_t));
            fileWriter.Write(&guid, sizeof(Guid));
            uint8_t assetNameLength = name.size();
            fileWriter.Write(&assetNameLength, sizeof(uint8_t));
            fileWriter.Write(name.data(), assetNameLength);
            int textLength = text.size();
            fileWriter.Write(&textLength, sizeof(int));
            fileWriter.Write(text.data(), textLength);
            fileWriter.Flush();
        }

        const std::string& GetText() const
        {
            return text;
        }

        void SetText(std::string value)
        {
            text = std::move(value);
        }
    };

    TextAsset::Factory::Factory(ServiceCollection& serviceCollection)
        : Asset::Factory(serviceCollection)
    {
    }

    std::unique_ptr<TextAsset> TextAsset::Factory::Create() const
    {
        return std::make_unique<TextAsset>(std::make_unique<Impl>(Guid::New()));
    }

    std::unique_ptr<TextAsset> TextAsset::Factory::Create(const TextAsset& original) const
    {
        auto textAsset = Create();
        textAsset->SetName(original.GetName());
        textAsset->SetText(original.GetText());
        return textAsset;
    }

    std::unique_ptr<Asset> TextAsset::Factory::Create(FileReader& fileReader) const
    {
        Guid signature;
        fileReader.Read(&signature, sizeof(Guid));
        uint8_t serializerVersion;
        fileReader.Read(&serializerVersion, sizeof(uint8_t));
        uint8_t assetType;
        fileReader.Read(&assetType, sizeof(uint8_t));
        Guid assetId;
        fileReader.Read(&assetId, sizeof(Guid));
        uint8_t assetNameLength;
        fileReader.Read(&assetNameLength, sizeof(uint8_t));
        std::string assetName(assetNameLength, ' ');
        fileReader.Read(assetName.data(), assetNameLength);
        int textLength;
        fileReader.Read(&textLength, sizeof(int));
        std::string text(textLength, ' ');
        fileReader.Read(text.data(), textLength);

        auto textAsset = std::make_unique<TextAsset>(std::make_unique<Impl>(assetId));
        textAsset->SetName(assetName);
        textAsset->SetText(text);
        return textAsset;
    }

    TextAsset::~TextAsset() = default;

    TextAsset::TextAsset(std::unique_ptr<Impl> impl)
        : impl(std::move(impl))
    {
    }

    TextAsset::TextAsset(TextAsset&& other) noexcept = default;

    TextAsset& TextAsset::operator=(TextAsset&& rhs) noexcept = default;

    const Guid& TextAsset::GetId() const
    {
        return impl->GetId();
    }

    const std::string& TextAsset::GetName() const
    {
        return impl->GetName();
    }

    void TextAsset::SetName(const std::string& value)
    {
        impl->SetName(value);
    }

    void TextAsset::Dump(FileWriter& fileWriter) const
    {
        impl->Dump(fileWriter);
    }

    const std::string& TextAsset::GetText() const
    {
        return impl->GetText();
    }

    void TextAsset::SetText(std::string value)
    {
        impl->SetText(std::move(value));
    }
}
