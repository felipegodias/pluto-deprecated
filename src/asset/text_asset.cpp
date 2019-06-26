#include <pluto/asset/text_asset.h>
#include <pluto/guid.h>
#include <pluto/math/vector2.h>
#include <vector>

namespace pluto
{
    inline void Write(std::ostream& os, const void* ptr, const std::streamsize count)
    {
        os.write(reinterpret_cast<const char*>(ptr), count);
    }

    inline void Read(std::istream& is, void* ptr, const std::streamsize count)
    {
        is.read(reinterpret_cast<char*>(ptr), count);
    }

    class TextAsset::Impl
    {
    private:
        Guid guid;
        std::string name;
        std::string text;

    public:
        explicit Impl(Guid guid) : guid(std::move(guid))
        {
            Vector2<int> a;
            Vector2<float> b;

            bool c = a == b;
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

        void Dump(std::ostream& os) const
        {
            Write(os, &guid, sizeof(Guid));
            uint8_t serializerVersion = 1;
            Write(os, &serializerVersion, sizeof(uint8_t));
            uint8_t assetType = 1;
            Write(os, &assetType, sizeof(uint8_t));
            Write(os, &guid, sizeof(Guid));
            uint8_t assetNameLength = name.size();
            Write(os, &assetNameLength, sizeof(uint8_t));
            Write(os, name.data(), assetNameLength);
            int textLength = text.size();
            Write(os, &textLength, sizeof(int));
            Write(os, text.data(), textLength);
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
        return std::make_unique<TextAsset>(std::make_unique<Impl>(Guid::New()));
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
        Read(is, &signature, sizeof(Guid));
        uint8_t serializerVersion;
        Read(is, &serializerVersion, sizeof(uint8_t));
        uint8_t assetType;
        Read(is, &assetType, sizeof(uint8_t));
        Guid assetId;
        Read(is, &assetId, sizeof(Guid));
        uint8_t assetNameLength;
        Read(is, &assetNameLength, sizeof(uint8_t));
        std::string assetName(assetNameLength, ' ');
        Read(is, assetName.data(), assetNameLength);
        int textLength;
        Read(is, &textLength, sizeof(int));
        std::string text(textLength, ' ');
        Read(is, text.data(), textLength);

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

    void TextAsset::Dump(std::ostream& os) const
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
