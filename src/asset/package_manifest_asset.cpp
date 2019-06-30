#include <pluto/asset/package_manifest_asset.h>
#include <pluto/file/file_reader.h>
#include <pluto/file/file_writer.h>
#include <pluto/guid.h>

#include <string>
#include <unordered_set>
#include <unordered_map>

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

    class PackageManifestAsset::Impl
    {
    private:
        Guid guid;
        std::string name;
        std::unordered_set<Guid> guids;
        std::unordered_map<std::string, Guid> guidsByPath;

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

        void Dump(std::ostream& os) const
        {
            Write(os, &guid, sizeof(Guid));
            uint8_t serializerVersion = 1;
            Write(os, &serializerVersion, sizeof(uint8_t));
            uint8_t assetType = 0;
            Write(os, &assetType, sizeof(uint8_t));
            Write(os, &guid, sizeof(Guid));
            uint8_t assetNameLength = name.size();
            Write(os, &assetNameLength, sizeof(uint8_t));
            Write(os, name.data(), assetNameLength);

            uint16_t assetsCount = guidsByPath.size();
            Write(os, &assetsCount, sizeof(uint16_t));

            for (const auto& it : guidsByPath)
            {
                Write(os, &it.second, sizeof(Guid));
                uint16_t assetPathLength = it.first.size();
                Write(os, &assetPathLength, sizeof(uint16_t));
                Write(os, it.first.data(), assetPathLength);
            }

            os.flush();
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

    PackageManifestAsset::Factory::Factory(DiContainer& diContainer) : BaseFactory(diContainer)
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

    std::unique_ptr<PackageManifestAsset> PackageManifestAsset::Factory::Create(std::istream& is) const
    {
        Guid signature;
        Read(is, &signature, sizeof(Guid));
        uint8_t serializerVersion;
        Read(is, &serializerVersion, sizeof(uint8_t));
        uint8_t assetType;
        Read(is, &assetType, sizeof(uint8_t));

        Guid assetId;
        Read(is, &assetId, sizeof(Guid));

        auto instance = std::make_unique<PackageManifestAsset>(std::make_unique<Impl>(assetId));

        uint8_t assetNameLength;
        Read(is, &assetNameLength, sizeof(uint8_t));
        std::string assetName(assetNameLength, ' ');
        Read(is, assetName.data(), assetNameLength);
        instance->SetName(assetName);

        uint16_t assetsListCount;
        Read(is, &assetsListCount, sizeof(uint16_t));

        for (uint16_t i = 0; i < assetsListCount; ++i)
        {
            Guid assetListGuid;
            Read(is, &assetListGuid, sizeof(Guid));

            uint16_t assetVirtualPathLength;
            Read(is, &assetVirtualPathLength, sizeof(uint16_t));
            std::string assetVirtualPath(assetVirtualPathLength, ' ');
            Read(is, assetVirtualPath.data(), assetVirtualPathLength);
            instance->AddAsset(std::move(assetVirtualPath), assetListGuid);
        }

        return instance;
    }

    std::unique_ptr<PackageManifestAsset> PackageManifestAsset::Factory::Create(FileReader& fileReader) const
    {
        Guid signature;
        fileReader.Read(&signature, sizeof(Guid));
        uint8_t serializerVersion;
        fileReader.Read(&serializerVersion, sizeof(uint8_t));
        uint8_t assetType;
        fileReader.Read(&assetType, sizeof(uint8_t));

        Guid assetId;
        fileReader.Read(&assetId, sizeof(Guid));

        auto instance = std::make_unique<PackageManifestAsset>(std::make_unique<Impl>(assetId));

        uint8_t assetNameLength;
        fileReader.Read(&assetNameLength, sizeof(uint8_t));
        std::string assetName(assetNameLength, ' ');
        fileReader.Read(assetName.data(), assetNameLength);
        instance->SetName(assetName);

        uint16_t assetsListCount;
        fileReader.Read(&assetsListCount, sizeof(uint16_t));

        for (uint16_t i = 0; i < assetsListCount; ++i)
        {
            Guid assetListGuid;
            fileReader.Read(&assetListGuid, sizeof(Guid));

            uint16_t assetVirtualPathLength;
            fileReader.Read(&assetVirtualPathLength, sizeof(uint16_t));
            std::string assetVirtualPath(assetVirtualPathLength, ' ');
            fileReader.Read(assetVirtualPath.data(), assetVirtualPathLength);
            instance->AddAsset(std::move(assetVirtualPath), assetListGuid);
        }

        return instance;
    }

    PackageManifestAsset::PackageManifestAsset(std::unique_ptr<Impl> impl) : impl(std::move(impl))
    {
    }

    PackageManifestAsset::PackageManifestAsset(PackageManifestAsset&& other) noexcept : impl(std::move(other.impl))
    {
    }

    PackageManifestAsset::~PackageManifestAsset() = default;

    PackageManifestAsset& PackageManifestAsset::operator=(const PackageManifestAsset& rhs)
    {
        if (this == &rhs)
        {
            return *this;
        }

        impl->Clone(*rhs.impl);
        return *this;
    }

    PackageManifestAsset& PackageManifestAsset::operator=(PackageManifestAsset&& rhs) noexcept
    {
        if (this == &rhs)
        {
            return *this;
        }

        impl = std::move(rhs.impl);
        return *this;
    }

    const Guid& PackageManifestAsset::GetId() const
    {
        return impl->GetId();
    }

    const std::string& PackageManifestAsset::GetName() const
    {
        return impl->GetName();
    }

    void PackageManifestAsset::SetName(std::string name)
    {
        impl->SetName(std::move(name));
    }

    void PackageManifestAsset::Dump(std::ostream& os) const
    {
        impl->Dump(os);
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
