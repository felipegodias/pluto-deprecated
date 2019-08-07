#include <pluto/asset/asset_manager.h>
#include <pluto/file/file_manager.h>

#include <pluto/file/file_reader.h>
#include <pluto/file/path.h>

#include <pluto/service/service_collection.h>
#include <pluto/service/base_factory.h>
#include <pluto/guid.h>
#include <pluto/asset/asset.h>
#include <pluto/asset/package_manifest_asset.h>
#include <pluto/exception.h>

#include <pluto/event/event_manager.h>
#include <pluto/asset/events/on_asset_unload_event.h>

#include <pluto/memory/memory_manager.h>
#include <pluto/memory/resource.h>

#include <fmt/ostream.h>

#include <string>
#include <memory>
#include <typeindex>
#include <unordered_map>

namespace pluto
{
    class AssetManager::Impl
    {
        std::unordered_map<std::string, PackageManifestAsset*> manifests;

        MemoryManager* memoryManager;
        FileManager* fileManager;
        EventManager* eventManager;
        ServiceCollection* serviceCollection;

    public:
        Impl(MemoryManager& memoryManager, FileManager& fileManager, EventManager& eventManager,
             ServiceCollection& serviceCollection)
            : memoryManager(&memoryManager),
              fileManager(&fileManager),
              eventManager(&eventManager),
              serviceCollection(&serviceCollection)
        {
        }

        void LoadPackage(const std::string& name)
        {
            if (manifests.find(name) != manifests.end())
            {
                Exception::Throw(std::runtime_error(fmt::format("Package {0} already loaded.", name)));
            }

            const std::string physicalFilePath = fmt::format("packages/{0}/{0}", name);
            Resource<Asset> package = LoadFromFile(typeid(PackageManifestAsset), Path(physicalFilePath));
        }

        Resource<Asset> Load(const std::type_info& type, const Path& path)
        {
            const PackageManifestAsset* package = nullptr;
            for (const auto& manifest : manifests)
            {
                if (manifest.second->Contains(path.Str()))
                {
                    package = manifest.second;
                    break;
                }
            }

            if (package == nullptr)
            {
                return nullptr;
            }

            const Guid guid = package->GetAssetGuid(path.Str());

            const Resource<Object> resource = memoryManager->Get(guid);
            if (resource != nullptr)
            {
                return ResourceUtils::Cast<Asset>(resource);
            }

            const Path physicalFilePath(fmt::format("packages/{0}/{1}", package->GetName(), guid));
            return LoadFromFile(type, physicalFilePath);
        }

        Resource<Asset> Load(const std::type_info& type, const Guid& guid)
        {
            const Resource<Object> resource = memoryManager->Get(guid);
            if (resource != nullptr)
            {
                return ResourceUtils::Cast<Asset>(resource);
            }

            const PackageManifestAsset* package = nullptr;
            for (const auto& manifest : manifests)
            {
                if (manifest.second->Contains(guid))
                {
                    package = manifest.second;
                    break;
                }
            }

            if (package == nullptr)
            {
                return nullptr;
            }

            const std::string physicalFilePath = fmt::format("packages/{0}/{1}", package->GetName(), guid);
            return LoadFromFile(type, Path(physicalFilePath));
        }

        void Unload(const Asset& asset)
        {
            if (dynamic_cast<const PackageManifestAsset*>(&asset) != nullptr)
            {
                manifests.erase(asset.GetName());
            }

            eventManager->Dispatch(OnAssetUnloadEvent(asset));
            memoryManager->Remove(asset);
        }

        Resource<Asset> Register(std::unique_ptr<Asset> asset)
        {
            Asset* result = asset.get();
            Resource<Asset> resource = ResourceUtils::Cast<Asset>(memoryManager->Add(std::move(asset)));

            auto packageManifestAsset = dynamic_cast<PackageManifestAsset*>(result);
            if (packageManifestAsset != nullptr)
            {
                if (manifests.find(result->GetName()) != manifests.end())
                {
                    Exception::Throw(
                        std::runtime_error(fmt::format("Package with name {0} already registered.", result->GetId())));
                }

                manifests.emplace(result->GetName(), packageManifestAsset);
            }

            return resource;
        }

    private:
        Resource<Asset> LoadFromFile(const std::type_info& type, const Path& path)
        {
            if (!fileManager->Exists(path))
            {
                Exception::Throw(
                    std::runtime_error(fmt::format("Asset at path {0} does not exists.", path)));
            }

            const std::unique_ptr<FileReader> file = fileManager->OpenRead(path);
            const auto& factory = dynamic_cast<Asset::Factory&>(serviceCollection->GetFactory(type));
            return ResourceUtils::Cast<Asset>(Register(factory.Create(*file)));
        }
    };

    AssetManager::Factory::Factory(ServiceCollection& serviceCollection)
        : BaseFactory(serviceCollection)
    {
    }

    std::unique_ptr<AssetManager> AssetManager::Factory::Create() const
    {
        ServiceCollection& serviceCollection = GetServiceCollection();
        auto& memoryManager = serviceCollection.GetService<MemoryManager>();
        auto& fileManager = serviceCollection.GetService<FileManager>();
        auto& eventManager = serviceCollection.GetService<EventManager>();

        return std::make_unique<AssetManager>(
            std::make_unique<Impl>(memoryManager, fileManager, eventManager, serviceCollection));
    }

    AssetManager::~AssetManager() = default;

    AssetManager::AssetManager(std::unique_ptr<Impl> impl)
        : impl(std::move(impl))
    {
    }

    AssetManager::AssetManager(AssetManager&& other) noexcept = default;

    AssetManager& AssetManager::operator=(AssetManager&& rhs) noexcept = default;

    void AssetManager::LoadPackage(const std::string& name)
    {
        impl->LoadPackage(name);
    }

    Resource<Asset> AssetManager::Load(const std::type_info& type, const Path& path)
    {
        return impl->Load(type, path);
    }

    Resource<Asset> AssetManager::Load(const std::type_info& type, const Guid& guid)
    {
        return impl->Load(type, guid);
    }

    void AssetManager::Unload(const Asset& asset)
    {
        impl->Unload(asset);
    }

    Resource<Asset> AssetManager::RegisterAsset(std::unique_ptr<Asset> asset)
    {
        return impl->Register(std::move(asset));
    }
}
