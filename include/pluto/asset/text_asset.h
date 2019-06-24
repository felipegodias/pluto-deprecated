#pragma once

#include "asset.h"
#include "../di/base_factory.h"
#include <memory>
#include <string>

namespace pluto
{
    /*
     * File layout in disk. (Version 1)
     * +--------+---------------------------+
     * | Size   | Description               |
     * +--------+---------------------------+
     * | 16     | File signature.           |
     * | 1      | Serializer version.       |
     * | 1      | Asset type.               |
     * | 16     | Asset unique identifier.  |
     * | 1      | Asset name length.        |
     * | Custom | Asset name.               |
     * +--------+---------------------------+
     * | 4      | Text length.              |
     * | Custom | Text.                     |
     * +--------+---------------------------+
     */
    class PLUTO_API TextAsset final : public Asset
    {
    public:
        class PLUTO_API Factory final : public BaseFactory 
        {
        public:
            explicit Factory(DiContainer& diContainer);
            std::unique_ptr<TextAsset> Create() const;
            std::unique_ptr<TextAsset> Create(const TextAsset& original) const;
            std::unique_ptr<TextAsset> Create(std::istream& is) const;
        };

    private:
        class Impl;
        std::unique_ptr<Impl> impl;

    public:
        explicit TextAsset(std::unique_ptr<Impl> impl);
        TextAsset(const TextAsset& other) = delete;
        TextAsset(TextAsset&& other) noexcept = delete;
        ~TextAsset() override;

        TextAsset& operator=(const TextAsset& rhs);
        TextAsset& operator=(TextAsset&& rhs) noexcept = delete;

        const Guid& GetId() const override;
        const std::string& GetName() const override;
        void SetName(std::string name) override;
        void Dump(std::ostream& os) override;

        const std::string& GetText() const;
        void SetText(std::string text);
    };
}
