#include <pluto/asset/asset_installer.h>
#include <pluto/asset/asset_manager.h>
#include <pluto/asset/text_asset.h>
#include <pluto/asset/mesh_asset.h>
#include <pluto/di/di_container.h>

namespace pluto
{
    void AssetInstaller::Install(DiContainer& diContainer)
    {
        diContainer.AddSingleton(std::make_unique<MeshAsset::Factory>(diContainer));
        diContainer.AddSingleton(std::make_unique<TextAsset::Factory>(diContainer));

        const AssetManager::Factory factory(diContainer);
        diContainer.AddSingleton(factory.Create());
    }

    void AssetInstaller::Uninstall(DiContainer& diContainer)
    {
        diContainer.RemoveSingleton<AssetManager>();
        diContainer.RemoveSingleton<MeshAsset::Factory>();
    }
}
