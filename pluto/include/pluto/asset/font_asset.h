#pragma once

#include "asset.h"
#include "pluto/service/base_factory.h"

namespace pluto
{
    template <typename T, typename Enable = void>
    class Resource;

    class TextureAsset;

    class PLUTO_API FontAsset final : public Asset
    {
    public:
        class PLUTO_API Factory final : public Asset::Factory
        {
        public:
            explicit Factory(ServiceCollection& serviceCollection);
            std::unique_ptr<Asset> Create(FileReader& fileReader) const override;
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
        void Dump(FileWriter& fileWriter) const override;
    };
}
