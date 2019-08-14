#include "pluto/asset/font_asset.h"
#include "pluto/guid.h"

namespace pluto
{
    class FontAsset::Impl
    {
    };

    FontAsset::Factory::Factory(ServiceCollection& serviceCollection)
        : Asset::Factory(serviceCollection)
    {
    }

    std::unique_ptr<Asset> FontAsset::Factory::Create(FileReader& fileReader) const
    {
        return std::make_unique<FontAsset>(std::make_unique<Impl>());
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
        return Guid();
    }

    const std::string& FontAsset::GetName() const
    {
        return "";
    }

    void FontAsset::SetName(const std::string& value)
    {
    }

    void FontAsset::Dump(FileWriter& fileWriter) const
    {
    }
}
