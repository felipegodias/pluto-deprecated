#include <pluto/asset/asset_installer.h>
#include <pluto/asset/asset_manager.h>
#include <pluto/asset/font_asset.h>
#include <pluto/asset/package_manifest_asset.h>
#include <pluto/asset/text_asset.h>
#include <pluto/asset/mesh_asset.h>
#include <pluto/asset/shader_asset.h>
#include <pluto/asset/material_asset.h>
#include <pluto/asset/texture_asset.h>
#include <pluto/service/service_collection.h>

namespace pluto
{
    void AssetInstaller::Install(ServiceCollection& serviceCollection)
    {
        serviceCollection.AddFactory<PackageManifestAsset>(
            std::make_unique<PackageManifestAsset::Factory>(serviceCollection));
        serviceCollection.AddFactory<MeshAsset>(std::make_unique<MeshAsset::Factory>(serviceCollection));
        serviceCollection.AddFactory<TextAsset>(std::make_unique<TextAsset::Factory>(serviceCollection));
        serviceCollection.AddFactory<ShaderAsset>(std::make_unique<ShaderAsset::Factory>(serviceCollection));
        serviceCollection.AddFactory<MaterialAsset>(std::make_unique<MaterialAsset::Factory>(serviceCollection));
        serviceCollection.AddFactory<TextureAsset>(std::make_unique<TextureAsset::Factory>(serviceCollection));
        serviceCollection.EmplaceFactory<FontAsset, FontAsset::Factory>();

        serviceCollection.AddService(AssetManager::Factory(serviceCollection).Create());
    }

    void AssetInstaller::Uninstall(ServiceCollection& serviceCollection)
    {
        serviceCollection.RemoveService<AssetManager>();
        serviceCollection.RemoveFactory<FontAsset>();
        serviceCollection.RemoveFactory<TextureAsset>();
        serviceCollection.RemoveFactory<MaterialAsset>();
        serviceCollection.RemoveFactory<ShaderAsset>();
        serviceCollection.RemoveFactory<TextAsset>();
        serviceCollection.RemoveFactory<MeshAsset>();
        serviceCollection.RemoveFactory<PackageManifestAsset>();
    }
}
