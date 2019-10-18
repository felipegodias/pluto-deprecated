#include <pluto/asset/package_manifest_asset.h>
#include <pluto/file/reader.h>
#include <pluto/file/file_writer.h>
#include <pluto/guid.h>

#include <string>
#include <unordered_set>
#include <unordered_map>

namespace pluto
{
    class PackageManifestAsset::Impl
    {
    private:
        Guid guid;
        std::string name;
        std::unordered_set<Guid> guids;
        std::unordered_map<std::string, Guid> guidsByPath;

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

        void SetName(const std::string& value)
        {
            name = value;
        }

        void Dump(FileWriter& fileWriter) const
        {
            fileWriter.Write(&guid, sizeof(Guid));
            uint8_t serializerVersion = 1;
            fileWriter.Write(&serializerVersion, sizeof(uint8_t));
            uint8_t assetType = 0;
            fileWriter.Write(&assetType, sizeof(uint8_t));
            fileWriter.Write(&guid, sizeof(Guid));
            uint8_t assetNameLength = name.size();
            fileWriter.Write(&assetNameLength, sizeof(uint8_t));
            fileWriter.Write(name.data(), assetNameLength);

            uint16_t assetsCount = guidsByPath.size();
            fileWriter.Write(&assetsCount, sizeof(uint16_t));

            for (const auto& it : guidsByPath)
            {
                fileWriter.Write(&it.second, sizeof(Guid));
                uint16_t assetPathLength = it.first.size();
                fileWriter.Write(&assetPathLength, sizeof(uint16_t));
                fileWriter.Write(it.first.data(), assetPathLength);
            }

            fileWriter.Flush();
        }

        bool Contains(const std::string& virtualPath) const
        {
            return guidsByPath.find(virtualPath) != guidsByPath.end();
        }

        bool Contains(const Guid& guid) const
        {
            return guids.find(guid) != guids.end();
        }

        void Clear()
        {
            guids.clear();
            guidsByPath.clear();
        }

        void AddAsset(std::string virtualPath, Guid guid)
        {
            if (Contains(virtualPath))
            {
                throw std::runtime_error("");
            }

            if (Contains(guid))
            {
                throw std::runtime_error("");
            }

            guids.emplace(guid);
            guidsByPath[std::move(virtualPath)] = std::move(guid);
        }

        const Guid& GetAssetGuid(const std::string& virtualPath) const
        {
            const auto& it = guidsByPath.find(virtualPath);
            if (it == guidsByPath.end())
            {
                throw std::runtime_error("");
            }

            return it->second;
        }

        std::vector<std::string> GetAssets() const
        {
            std::vector<std::string> paths(guidsByPath.size());
            int i = 0;
            for (const auto& it : guidsByPath)
            {
                paths[i++] = it.first;
            }
            return paths;
        }

        void Clone(const Impl& other)
        {
            name = other.name;
            guids = other.guids;
            guidsByPath = other.guidsByPath;
        }
    };

    PackageManifestAsset::Factory::Factory(ServiceCollection& serviceCollection)
        : Asset::Factory(serviceCollection)
    {
    }

    std::unique_ptr<PackageManifestAsset> PackageManifestAsset::Factory::Create() const
    {
        return std::make_unique<PackageManifestAsset>(std::make_unique<Impl>(Guid::New()));
    }

    std::unique_ptr<PackageManifestAsset> PackageManifestAsset::Factory::Create(
        const PackageManifestAsset& original) const
    {
        std::unique_ptr<PackageManifestAsset> instance = Create();
        return instance;
    }

    std::unique_ptr<Asset> PackageManifestAsset::Factory::Create(Reader& reader) const
    {
        Guid signature;
        reader.Read(&signature, sizeof(Guid));
        uint8_t serializerVersion;
        reader.Read(&serializerVersion, sizeof(uint8_t));
        uint8_t assetType;
        reader.Read(&assetType, sizeof(uint8_t));

        Guid assetId;
        reader.Read(&assetId, sizeof(Guid));

        auto instance = std::make_unique<PackageManifestAsset>(std::make_unique<Impl>(assetId));

        uint8_t assetNameLength;
        reader.Read(&assetNameLength, sizeof(uint8_t));
        std::string assetName(assetNameLength, ' ');
        reader.Read(assetName.data(), assetNameLength);
        instance->SetName(assetName);

        uint16_t assetsListCount;
        reader.Read(&assetsListCount, sizeof(uint16_t));

        for (uint16_t i = 0; i < assetsListCount; ++i)
        {
            Guid assetListGuid;
            reader.Read(&assetListGuid, sizeof(Guid));

            uint16_t assetVirtualPathLength;
            reader.Read(&assetVirtualPathLength, sizeof(uint16_t));
            std::string assetVirtualPath(assetVirtualPathLength, ' ');
            reader.Read(assetVirtualPath.data(), assetVirtualPathLength);
            instance->AddAsset(std::move(assetVirtualPath), assetListGuid);
        }

        return instance;
    }

    PackageManifestAsset::~PackageManifestAsset() = default;

    PackageManifestAsset::PackageManifestAsset(std::unique_ptr<Impl> impl)
        : impl(std::move(impl))
    {
    }

    PackageManifestAsset::PackageManifestAsset(PackageManifestAsset&& other) noexcept = default;

    PackageManifestAsset& PackageManifestAsset::operator=(PackageManifestAsset&& rhs) noexcept = default;

    const Guid& PackageManifestAsset::GetId() const
    {
        return impl->GetId();
    }

    const std::string& PackageManifestAsset::GetName() const
    {
        return impl->GetName();
    }

    void PackageManifestAsset::SetName(const std::string& value)
    {
        impl->SetName(value);
    }

    void PackageManifestAsset::Dump(FileWriter& fileWriter) const
    {
        impl->Dump(fileWriter);
    }

    bool PackageManifestAsset::Contains(const std::string& virtualPath) const
    {
        return impl->Contains(virtualPath);
    }

    bool PackageManifestAsset::Contains(const Guid& guid) const
    {
        return impl->Contains(guid);
    }

    void PackageManifestAsset::Clear()
    {
        impl->Clear();
    }

    void PackageManifestAsset::AddAsset(std::string virtualPath, Guid guid)
    {
        impl->AddAsset(std::move(virtualPath), std::move(guid));
    }

    const Guid& PackageManifestAsset::GetAssetGuid(const std::string& virtualPath) const
    {
        return impl->GetAssetGuid(virtualPath);
    }

    std::vector<std::string> PackageManifestAsset::GetAssets() const
    {
        return impl->GetAssets();
    }
}
