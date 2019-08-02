#include <pluto/asset/asset_manager.h>
#include <pluto/file/file_manager.h>

#include <pluto/file/file_reader.h>
#include <pluto/file/path.h>

#include <pluto/service/service_collection.h>
#include <pluto/service/base_factory.h>
#include <pluto/guid.h>
#include <pluto/asset/asset.h>
#include <pluto/asset/package_manifest_asset.h>
#include <pluto/asset/text_asset.h>
#include <pluto/asset/mesh_asset.h>
#include <pluto/asset/shader_asset.h>
#include <pluto/asset/material_asset.h>
#include <pluto/asset/texture_asset.h>
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
        std::unordered_map<std::type_index, const Asset::Factory*> factories;

        MemoryManager* memoryManager;
        const FileManager* fileManager;
        const EventManager* eventManager;

    public:
        Impl(MemoryManager& memoryManager, const FileManager& fileManager, const EventManager& eventManager,
             const PackageManifestAsset::Factory& packageManifestFactory, const TextAsset::Factory& textFactory,
             const MeshAsset::Factory& meshFactory, const ShaderAsset::Factory& shaderFactory,
             const TextureAsset::Factory& textureFactory)
            : memoryManager(&memoryManager),
              fileManager(&fileManager),
              eventManager(&eventManager)
        {
            factories.emplace(typeid(PackageManifestAsset), &packageManifestFactory);
            factories.emplace(typeid(TextAsset), &textFactory);
            factories.emplace(typeid(MeshAsset), &meshFactory);
            factories.emplace(typeid(ShaderAsset), &shaderFactory);
            factories.emplace(typeid(TextureAsset), &textureFactory);
        }

        void LoadPackage(const std::string& name)
        {
            if (manifests.find(name) != manifests.end())
            {
                Exception::Throw(std::runtime_error(fmt::format("Package {0} already loaded.", name)));
            }

            const std::string physicalFilePath = fmt::format("packages/{0}/{0}", name);
            LoadFromFile(typeid(PackageManifestAsset), Path(physicalFilePath));
        }

        std::shared_ptr<Resource<Asset>> Load(const std::type_index& type, const Path& path)
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
            const std::shared_ptr<Resource<Object>> resource = memoryManager->Get(guid);
            if (resource != nullptr)
            {
                ResourcePointerCast<Asset, Object>(resource);
            }

            const Path physicalFilePath(fmt::format("packages/{0}/{1}", package->GetName(), guid));
            return LoadFromFile(type, physicalFilePath);
        }

        Asset* Load(const std::type_index& type, const Guid& guid)
        {
            Object* object = memoryManager->Get(guid);
            if (object != nullptr)
            {
                return dynamic_cast<Asset*>(object);
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
            memoryManager->Remove(asset.GetId());
        }

        Asset& Register(std::unique_ptr<Asset> asset)
        {
            Asset* result = asset.get();
            memoryManager->Add(std::move(asset));

            auto packageManifestAsset = dynamic_cast<PackageManifestAsset*>(result);
            if (packageManifestAsset != nullptr)
            {
                if (manifests.find(result->GetName()) != manifests.end())
                {
                    Exception::Throw(
                        std::runtime_error(fmt::format("Package with name {0} already registered.", asset->GetId())));
                }

                manifests.emplace(result->GetName(), packageManifestAsset);
            }

            return *result;
        }

    private:
        Asset* LoadFromFile(const std::type_index& type, const Path& path)
        {
            if (!fileManager->Exists(path))
            {
                Exception::Throw(
                    std::runtime_error(fmt::format("Asset at path {0} does not exists.", path)));
            }

            const std::unique_ptr<FileReader> file = fileManager->OpenRead(path);
            const auto factory = factories.at(type);
            return &Register(factory->Create(*file));
        }
    };

    AssetManager::Factory::Factory(ServiceCollection& diContainer)
        : BaseFactory(diContainer)
    {
    }

    std::unique_ptr<AssetManager> AssetManager::Factory::Create() const
    {
        ServiceCollection& serviceCollection = GetServiceCollection();
        auto& memoryManager = serviceCollection.GetService<MemoryManager>();
        const auto& fileManager = serviceCollection.GetService<FileManager>();
        const auto& eventManager = serviceCollection.GetService<EventManager>();
        const auto& packageManifestFactory = serviceCollection.GetService<PackageManifestAsset::Factory>();
        const auto& textFactory = serviceCollection.GetService<TextAsset::Factory>();
        const auto& meshFactory = serviceCollection.GetService<MeshAsset::Factory>();
        const auto& shaderFactory = serviceCollection.GetService<ShaderAsset::Factory>();
        const auto& textureFactory = serviceCollection.GetService<TextureAsset::Factory>();
        return std::make_unique<AssetManager>(std::make_unique<Impl>(memoryManager, fileManager, eventManager,
                                                                     packageManifestFactory,
                                                                     textFactory, meshFactory, shaderFactory,
                                                                     textureFactory));
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

    Asset* AssetManager::Load(const std::type_index& type, const Path& path)
    {
        return impl->Load(type, path);
    }

    Asset* AssetManager::Load(const std::type_index& type, const Guid& guid)
    {
        return impl->Load(type, guid);
    }

    void AssetManager::Unload(const Asset& asset)
    {
        impl->Unload(asset);
    }

    Asset& AssetManager::RegisterAsset(std::unique_ptr<Asset> asset)
    {
        return impl->Register(std::move(asset));
    }
}
