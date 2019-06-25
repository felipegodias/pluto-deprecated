#include <pluto/asset/asset_manager.h>
#include <pluto/file/file_manager.h>
#include <pluto/di/di_container.h>
#include <pluto/di/base_factory.h>
#include <pluto/guid.h>
#include <pluto/asset/asset.h>
#include <pluto/asset/package_manifest_asset.h>
#include <pluto/asset/text_asset.h>
#include <pluto/asset/mesh_asset.h>

#include <string>
#include <fmt/ostream.h>
#include <memory>
#include <typeindex>
#include <unordered_map>

namespace pluto
{
    class AssetManager::Impl
    {
    private:
        const FileManager& fileManager;

        std::unordered_map<std::string, const PackageManifestAsset&> manifests;
        std::unordered_map<std::type_index, const BaseFactory&> factories;
        std::unordered_map<Guid, std::unique_ptr<Asset>> loadedAssets;

    public:
        Impl(const FileManager& fileManager, const PackageManifestAsset::Factory& packageManifestFactory,
             const TextAsset::Factory& textFactory, const MeshAsset::Factory& meshFactory) : fileManager(fileManager)
        {
            factories.emplace(typeid(PackageManifestAsset), packageManifestFactory);
            factories.emplace(typeid(TextAsset), textFactory);
            factories.emplace(typeid(MeshAsset), meshFactory);
        }

        void LoadPackage(const std::string& name)
        {
            if (manifests.find(name) != manifests.end())
            {
                throw std::runtime_error("");
            }

            const std::string physicalFilePath = fmt::format("packages/{0}/{0}", name);
            LoadFromFile<PackageManifestAsset>(physicalFilePath);
        }

        template <typename T, IsAsset<T>  = 0>
        T& Load(const std::string& path)
        {
            const PackageManifestAsset* package = nullptr;
            for (const auto& manifest : manifests)
            {
                if (manifest.second.Contains(path))
                {
                    package = &manifest.second;
                    break;
                }
            }

            if (package == nullptr)
            {
                throw std::runtime_error("");
            }

            const Guid guid = package->GetAssetGuid(path);
            const auto& it = loadedAssets.find(guid);
            if (it != loadedAssets.end())
            {
                return static_cast<T&>(*it->second);
            }

            const std::string physicalFilePath = fmt::format("packages/{0}/{1}", package->GetName(), guid);
            return LoadFromFile<T>(physicalFilePath);
        }

        template <typename T, IsAsset<T>  = 0>
        T& Load(const Guid& guid)
        {
            const auto& it = loadedAssets.find(guid);
            if (it != loadedAssets.end())
            {
                return static_cast<T&>(*it->second);
            }

            const PackageManifestAsset* package = nullptr;
            for (const auto& manifest : manifests)
            {
                if (manifest.second.Contains(guid))
                {
                    package = &manifest.second;
                    break;
                }
            }

            if (package == nullptr)
            {
                throw std::runtime_error("");
            }

            const std::string physicalFilePath = fmt::format("packages/{0}/{1}", package->GetName(), guid);
            return LoadFromFile<T>(physicalFilePath);
        }

        template <typename T, IsAsset<T>  = 0>
        void Unload(const T& asset)
        {
            const auto& it = loadedAssets.find(asset.GetId());
            if (it == loadedAssets.end())
            {
                throw std::runtime_error("");
            }

            if constexpr (std::is_same<T, PackageManifestAsset>::value)
            {
                manifests.erase(asset.GetName());
            }

            loadedAssets.erase(it);
        }

        template <typename T, IsAsset<T>  = 0>
        T& Register(std::unique_ptr<T> asset)
        {
            T& result = *asset;
            loadedAssets.emplace(asset->GetId(), std::move(asset));
            if constexpr (std::is_same<T, PackageManifestAsset>::value)
            {
                if (manifests.find(result.GetName()) != manifests.end())
                {
                    throw std::runtime_error("");
                }

                manifests.emplace(result.GetName(), result);
            }

            return result;
        }

        std::vector<Asset*> GetLoadedAssets() const
        {
            std::vector<Asset*> result(loadedAssets.size());
            int i = 0;
            for (const auto& loadedAsset : loadedAssets)
            {
                result[i++] = loadedAsset.second.get();
            }
            return result;
        }

    private:
        template <typename T, IsAsset<T>  = 0>
        T& LoadFromFile(const std::string& physicalFilePath)
        {
            if (!fileManager.Exists(physicalFilePath))
            {
                throw std::runtime_error("");
            }

            std::ifstream file = fileManager.OpenRead(physicalFilePath);
            const auto& factory = static_cast<const typename T::Factory&>(factories.at(typeid(T)));
            std::unique_ptr<T> asset = factory.Create(file);
            return Register(std::move(asset));
        }
    };

    AssetManager::Factory::Factory(DiContainer& diContainer) : BaseFactory(diContainer)
    {
    }

    std::unique_ptr<AssetManager> AssetManager::Factory::Create() const
    {
        const auto& fileManager = diContainer.GetSingleton<FileManager>();
        const auto& packageManifestFactory = diContainer.GetSingleton<PackageManifestAsset::Factory>();
        const auto& textFactory = diContainer.GetSingleton<TextAsset::Factory>();
        const auto& meshFactory = diContainer.GetSingleton<MeshAsset::Factory>();
        return std::make_unique<AssetManager>(
            std::make_unique<Impl>(fileManager, packageManifestFactory, textFactory, meshFactory));
    }

    AssetManager::AssetManager(std::unique_ptr<Impl> impl) : impl(std::move(impl))
    {
    }

    AssetManager::~AssetManager() = default;

    void AssetManager::LoadPackage(const std::string& name)
    {
        impl->LoadPackage(name);
    }

    template <typename T, IsAsset<T>>
    T& AssetManager::Load(const std::string& path)
    {
        return impl->Load<T>(path);
    }

    template <typename T, IsAsset<T>>
    T& AssetManager::Load(const Guid& guid)
    {
        return impl->Load<T>(guid);
    }

    template <typename T, IsAsset<T>>
    void AssetManager::Unload(const T& asset)
    {
        impl->Unload<T>(asset);
    }

    template <typename T, IsAsset<T>>
    T& AssetManager::Register(std::unique_ptr<T> asset)
    {
        return impl->Register(std::move(asset));
    }

    std::vector<Asset*> AssetManager::GetLoadedAssets() const
    {
        return impl->GetLoadedAssets();
    }

    template PackageManifestAsset& AssetManager::Load(const std::string& path);
    template PackageManifestAsset& AssetManager::Load(const Guid& guid);
    template PackageManifestAsset& AssetManager::Register(std::unique_ptr<PackageManifestAsset> asset);
    template void AssetManager::Unload(const PackageManifestAsset& asset);

    template TextAsset& AssetManager::Load(const std::string& path);
    template TextAsset& AssetManager::Load(const Guid& guid);
    template TextAsset& AssetManager::Register(std::unique_ptr<TextAsset> asset);
    template void AssetManager::Unload(const TextAsset& asset);

    template MeshAsset& AssetManager::Load(const std::string& path);
    template MeshAsset& AssetManager::Load(const Guid& guid);
    template MeshAsset& AssetManager::Register(std::unique_ptr<MeshAsset> asset);
    template void AssetManager::Unload(const MeshAsset& asset);
}
