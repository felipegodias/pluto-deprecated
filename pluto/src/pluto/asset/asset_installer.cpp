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
        diContainer.AddFactory<PackageManifestAsset>(std::make_unique<PackageManifestAsset::Factory>(diContainer));
        diContainer.AddFactory<MeshAsset>(std::make_unique<MeshAsset::Factory>(diContainer));
        diContainer.AddFactory<TextAsset>(std::make_unique<TextAsset::Factory>(diContainer));
        diContainer.AddFactory<ShaderAsset>(std::make_unique<ShaderAsset::Factory>(diContainer));
        diContainer.AddFactory<MaterialAsset>(std::make_unique<MaterialAsset::Factory>(diContainer));
        diContainer.AddFactory<TextureAsset>(std::make_unique<TextureAsset::Factory>(diContainer));

        diContainer.AddService(AssetManager::Factory(diContainer).Create());
    }

    void AssetInstaller::Uninstall(ServiceCollection& diContainer)
    {
        diContainer.RemoveService<AssetManager>();
        diContainer.RemoveFactory<TextureAsset>();
        diContainer.RemoveFactory<MaterialAsset>();
        diContainer.RemoveFactory<ShaderAsset>();
        diContainer.RemoveFactory<TextAsset>();
        diContainer.RemoveFactory<MeshAsset>();
        diContainer.RemoveFactory<PackageManifestAsset>();
    }
}
