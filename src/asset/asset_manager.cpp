#include <pluto/asset/asset_manager.h>
#include <pluto/file/file_manager.h>
#include <pluto/di/di_container.h>
#include <pluto/di/base_factory.h>
#include <pluto/guid.h>
#include <pluto/asset/asset.h>
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

        std::unordered_map<std::string, Guid> manifest;
        std::unordered_map<std::type_index, const BaseFactory&> factories;
        std::unordered_map<Guid, std::unique_ptr<Asset>> loadedAssets;

    public:
        Impl(const FileManager& fileManager, const MeshAsset::Factory& meshFactory) : fileManager(fileManager)
        {
            factories.emplace(typeid(MeshAsset), meshFactory);

            // TODO: Remove test code.
            manifest.emplace("foo", Guid("b798360e-7add-4a10-9045-301ee55dd228"));
            manifest.emplace("bar", Guid("f85e4413-a94d-4442-9eaf-1678f16af981"));
        }

        template <typename T, IsAsset<T>  = 0>
        T& Load(const std::string& path)
        {
            const auto& it = manifest.find(path);
            if (it == manifest.end())
            {
                throw std::runtime_error("");
            }

            return Load<T>(it->second);
        }

        template <typename T, IsAsset<T>  = 0>
        T& Load(const Guid& guid)
        {
            const auto& it = loadedAssets.find(guid);
            if (it != loadedAssets.end())
            {
                return static_cast<T&>(*it->second);
            }

            const std::string physicalFilePath = fmt::format("assets/{0}", guid);
            if (!fileManager.Exists(physicalFilePath))
            {
                throw std::runtime_error("");
            }

            std::ifstream file = fileManager.ReadFile(physicalFilePath);
            const auto& factory = static_cast<const typename T::Factory&>(factories.at(typeid(T)));
            std::unique_ptr<T> asset = factory.Create(file);
            return Register(std::move(asset));
        }

        void Unload(const Asset& asset)
        {
            const auto& it = loadedAssets.find(asset.GetId());
            if (it == loadedAssets.end())
            {
                throw std::runtime_error("");
            }

            loadedAssets.erase(it);
        }

        template <typename T, IsAsset<T>  = 0>
        T& Register(std::unique_ptr<T> asset)
        {
            T& result = *asset;
            loadedAssets.emplace(asset->GetId(), std::move(asset));
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
    };

    AssetManager::Factory::Factory(DiContainer& diContainer) : BaseFactory(diContainer)
    {
    }

    std::unique_ptr<AssetManager> AssetManager::Factory::Create() const
    {
        const auto& fileManager = diContainer.GetSingleton<FileManager>();
        const auto& meshFactory = diContainer.GetSingleton<MeshAsset::Factory>();
        return std::make_unique<AssetManager>(std::make_unique<Impl>(fileManager, meshFactory));
    }

    AssetManager::AssetManager(std::unique_ptr<Impl> impl) : impl(std::move(impl))
    {
    }

    AssetManager::~AssetManager() = default;

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

    void AssetManager::Unload(const Asset& asset)
    {
        impl->Unload(asset);
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

    template MeshAsset& AssetManager::Load(const std::string& path);
    template MeshAsset& AssetManager::Load(const Guid& guid);
    template MeshAsset& AssetManager::Register(std::unique_ptr<MeshAsset> asset);
}
