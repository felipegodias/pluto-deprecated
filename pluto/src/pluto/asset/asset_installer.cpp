#include <pluto/asset/asset_installer.h>
#include <pluto/asset/asset_manager.h>
#include <pluto/asset/package_manifest_asset.h>
#include <pluto/asset/text_asset.h>
#include <pluto/asset/mesh_asset.h>
#include <pluto/asset/shader_asset.h>
#include <pluto/asset/material_asset.h>
#include <pluto/asset/texture_asset.h>
#include <pluto/service/service_collection.h>

namespace pluto
{
    void AssetInstaller::Install(ServiceCollection& diContainer)
    {
        diContainer.AddService(std::make_unique<PackageManifestAsset::Factory>(diContainer));
        diContainer.AddService(std::make_unique<MeshAsset::Factory>(diContainer));
        diContainer.AddService(std::make_unique<TextAsset::Factory>(diContainer));
        diContainer.AddService(std::make_unique<ShaderAsset::Factory>(diContainer));
        diContainer.AddService(std::make_unique<MaterialAsset::Factory>(diContainer));
        diContainer.AddService(std::make_unique<TextureAsset::Factory>(diContainer));

        const AssetManager::Factory factory(diContainer);
        diContainer.AddService(factory.Create());
    }

    void AssetInstaller::Uninstall(ServiceCollection& diContainer)
    {
        diContainer.RemoveService<AssetManager>();
        diContainer.RemoveService<TextureAsset::Factory>();
        diContainer.RemoveService<MaterialAsset::Factory>();
        diContainer.RemoveService<ShaderAsset::Factory>();
        diContainer.RemoveService<TextAsset::Factory>();
        diContainer.RemoveService<MeshAsset::Factory>();
        diContainer.RemoveService<PackageManifestAsset::Factory>();
    }
}
