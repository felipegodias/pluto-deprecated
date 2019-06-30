#pragma once

#include "asset.h"
#include "../di/base_factory.h"
#include <memory>
#include <string>

namespace pluto
{
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
     * | int          | 4    | Text length.                 |
     * | string       | *    | Text.                        |
     * +--------------+------+------------------------------+
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
            std::unique_ptr<TextAsset> Create(FileReader& fileReader) const;
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
        void Dump(std::ostream& os) const override;
        void Dump(FileWriter& fileWriter) const override;

        const std::string& GetText() const;
        void SetText(std::string text);
    };
}
