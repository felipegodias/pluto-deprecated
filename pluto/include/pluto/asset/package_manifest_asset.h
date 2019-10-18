#pragma once

#include "asset.h"
#include "pluto/service/base_factory.h"

#include <string>
#include <memory>
#include <vector>

namespace pluto
{
    class FileStreamReader;

    /*
     * File layout in disk. (Version 1)
     * +--------------+------+------------------------------+
     * | Type         | Size | Description                  |
     * +--------------+------+------------------------------+
     * | GUID         | 1    | File signature.              |
     * | uint8_t      | 1    | Serializer version.          |
     * | uint8_t      | 1    | Asset type.                  |
     * | GUID         | 16   | Asset unique identifier.     |
     * | uint8_t      | 1    | Asset name length.           |
     * | string       | *    | Asset name.                  |
     * +--------------+------+------------------------------+
     * | uint16_t     | 2    | Assets count.                |
     * +==============+======+==============================+
     * | GUID         | 16   | Asset guid.                  |
     * | uint16_t     | 2    | Asset path length.           |
     * | string       | *    | Asset path.                  |
     * +--------------+------+------------------------------+
     */
    class PLUTO_API PackageManifestAsset final : public Asset
    {
    public:
        class PLUTO_API Factory final : public Asset::Factory
        {
        public:
            explicit Factory(ServiceCollection& serviceCollection);
            std::unique_ptr<PackageManifestAsset> Create() const;
            std::unique_ptr<PackageManifestAsset> Create(const PackageManifestAsset& original) const;
            std::unique_ptr<Asset> Create(StreamReader& reader) const override;
        };

    private:
        class Impl;
        std::unique_ptr<Impl> impl;

    public:
        ~PackageManifestAsset() override;
        explicit PackageManifestAsset(std::unique_ptr<Impl> impl);

        PackageManifestAsset(const PackageManifestAsset& other) = delete;
        PackageManifestAsset(PackageManifestAsset&& other) noexcept;
        PackageManifestAsset& operator=(const PackageManifestAsset& rhs) = delete;
        PackageManifestAsset& operator=(PackageManifestAsset&& rhs) noexcept;

        const Guid& GetId() const override;
        const std::string& GetName() const override;
        void SetName(const std::string& value) override;

        void Dump(FileStreamWriter& fileWriter) const override;

        bool Contains(const std::string& virtualPath) const;
        bool Contains(const Guid& guid) const;

        void Clear();

        void AddAsset(std::string virtualPath, Guid guid);
        const Guid& GetAssetGuid(const std::string& virtualPath) const;
        std::vector<std::string> GetAssets() const;
    };
}
