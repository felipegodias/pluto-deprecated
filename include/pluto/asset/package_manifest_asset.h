#pragma once

#include "asset.h"
#include "../di/base_factory.h"
#include <string>
#include <memory>
#include <vector>

namespace pluto
{
    class Asset;
    class Guid;

    /*
     * File layout in disk. (Version 1)
     * +------+------------------------------+
     * | Size | Description                  |
     * +------+------------------------------+
     * | 1    | File signature.              |
     * | 1    | Serializer version.          |
     * | 1    | Asset type.                  |
     * | 16   | Asset unique identifier.     |
     * | 1    | Asset name length.           |
     * | *    | Asset name.                  |
     * +------+------------------------------+
     * | 2    | Assets count.                |
     * +======+==============================+
     * | 16   | Asset guid.                  |
     * | 2    | Asset path length.           |
     * | *    | Asset path.                  |
     * +------+------------------------------+
     */
    class PLUTO_API PackageManifestAsset final : public Asset
    {
    public:
        class PLUTO_API Factory final : public BaseFactory
        {
        public:
            explicit Factory(DiContainer& diContainer);
            std::unique_ptr<PackageManifestAsset> Create() const;
            std::unique_ptr<PackageManifestAsset> Create(const PackageManifestAsset& original) const;
            std::unique_ptr<PackageManifestAsset> Create(std::istream& is) const;
        };

    private:
        class Impl;
        std::unique_ptr<Impl> impl;

    public:
        explicit PackageManifestAsset(std::unique_ptr<Impl> impl);
        PackageManifestAsset(const PackageManifestAsset& other) = delete;
        PackageManifestAsset(PackageManifestAsset&& other) noexcept;
        ~PackageManifestAsset() override;

        PackageManifestAsset& operator=(const PackageManifestAsset& rhs);
        PackageManifestAsset& operator=(PackageManifestAsset&& rhs) noexcept;

        const Guid& GetId() const override;
        const std::string& GetName() const override;
        void SetName(std::string name) override;
        void Dump(std::ostream& os) const override;

        bool Contains(const std::string& virtualPath) const;
        bool Contains(const Guid& guid) const;

        void Clear();

        void AddAsset(std::string virtualPath, Guid guid);
        const Guid& GetAssetGuid(const std::string& virtualPath) const;
        std::vector<std::string> GetAssets() const;
    };
}
