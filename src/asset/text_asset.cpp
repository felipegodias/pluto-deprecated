#include <pluto/asset/text_asset.h>
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
        explicit Impl(Guid guid) : guid(std::move(guid))
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

        void SetName(std::string name)
        {
            this->name = std::move(name);
        }

        void Dump(std::ostream& os)
        {
            os.write(reinterpret_cast<char*>(&guid), sizeof(Guid));
            uint8_t serializerVersion = 1;
            os.write(reinterpret_cast<char*>(&serializerVersion), sizeof(uint8_t));
            uint8_t assetType = 1;
            os.write(reinterpret_cast<char*>(&assetType), sizeof(uint8_t));
            os.write(reinterpret_cast<char*>(&guid), sizeof(Guid));
            uint8_t assetNameLength = name.size();
            os.write(reinterpret_cast<char*>(&assetNameLength), sizeof(uint8_t));
            os.write(reinterpret_cast<char*>(name.data()), assetNameLength);
            int textLength = text.size();
            os.write(reinterpret_cast<char*>(&textLength), sizeof(int));
            os.write(reinterpret_cast<char*>(text.data()), textLength);
            os.flush();
        }

        const std::string& GetText() const
        {
            return text;
        }

        void SetText(std::string text)
        {
            this->text = std::move(text);
        }
    };

    TextAsset::Factory::Factory(DiContainer& diContainer) : BaseFactory(diContainer)
    {
    }

    std::unique_ptr<TextAsset> TextAsset::Factory::Create() const
    {
        return std::make_unique<TextAsset>(std::make_unique<Impl>(Guid()));
    }

    std::unique_ptr<TextAsset> TextAsset::Factory::Create(const TextAsset& original) const
    {
        auto textAsset = Create();
        textAsset->SetName(original.GetName());
        textAsset->SetText(original.GetText());
        return textAsset;
    }

    std::unique_ptr<TextAsset> TextAsset::Factory::Create(std::istream& is) const
    {
        Guid signature;
        is.read(reinterpret_cast<char*>(&signature), sizeof(Guid));
        uint8_t serializerVersion;
        is.read(reinterpret_cast<char*>(&serializerVersion), sizeof(uint8_t));
        uint8_t assetType;
        is.read(reinterpret_cast<char*>(&assetType), sizeof(uint8_t));
        Guid assetId;
        is.read(reinterpret_cast<char*>(&assetId), sizeof(Guid));
        uint8_t assetNameLength;
        is.read(reinterpret_cast<char*>(&assetNameLength), sizeof(uint8_t));
        std::string assetName(assetNameLength, ' ');
        is.read(reinterpret_cast<char*>(assetName.data()), assetNameLength);
        int textLength;
        is.read(reinterpret_cast<char*>(&textLength), sizeof(int));
        std::string text(assetNameLength, ' ');
        is.read(reinterpret_cast<char*>(text.data()), textLength);

        auto textAsset = std::make_unique<TextAsset>(std::make_unique<Impl>(assetId));
        textAsset->SetName(assetName);
        textAsset->SetText(text);
        return textAsset;
    }

    TextAsset::TextAsset(std::unique_ptr<Impl> impl) : impl(std::move(impl))
    {
    }

    TextAsset::~TextAsset() = default;

    TextAsset& TextAsset::operator=(const TextAsset& rhs)
    {
        if (this == &rhs)
        {
            return *this;
        }

        this->SetName(rhs.GetName());
        this->SetText(rhs.GetText());
        return *this;
    }

    const Guid& TextAsset::GetId() const
    {
        return impl->GetId();
    }

    const std::string& TextAsset::GetName() const
    {
        return impl->GetName();
    }

    void TextAsset::SetName(std::string name)
    {
        impl->SetName(std::move(name));
    }

    void TextAsset::Dump(std::ostream& os)
    {
        impl->Dump(os);
    }

    const std::string& TextAsset::GetText() const
    {
        return impl->GetText();
    }

    void TextAsset::SetText(std::string text)
    {
        impl->SetText(std::move(text));
    }
}
