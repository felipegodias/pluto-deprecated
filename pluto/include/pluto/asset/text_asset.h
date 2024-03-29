#pragma once

#include "asset.h"
#include "pluto/service/base_factory.h"

#include <memory>
#include <string>

namespace pluto
{
    class FileStreamReader;

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
     * | int          | 4    | Text length.                 |
     * | string       | *    | Text.                        |
     * +--------------+------+------------------------------+
     */
    class PLUTO_API TextAsset final : public Asset
    {
    public:
        class PLUTO_API Factory final : public Asset::Factory
        {
        public:
            explicit Factory(ServiceCollection& serviceCollection);
            std::unique_ptr<TextAsset> Create() const;
            std::unique_ptr<TextAsset> Create(const TextAsset& original) const;
            std::unique_ptr<Asset> Create(StreamReader& reader) const override;
        };

    private:
        class Impl;
        std::unique_ptr<Impl> impl;

    public:
        ~TextAsset() override;
        explicit TextAsset(std::unique_ptr<Impl> impl);

        TextAsset(const TextAsset& other) = delete;
        TextAsset(TextAsset&& other) noexcept;
        TextAsset& operator=(const TextAsset& rhs) = delete;
        TextAsset& operator=(TextAsset&& rhs) noexcept;

        const Guid& GetId() const override;
        const std::string& GetName() const override;
        void SetName(const std::string& value) override;

        void Dump(FileStreamWriter& fileWriter) const override;

        const std::string& GetText() const;
        void SetText(const std::string& value);
    };
}
