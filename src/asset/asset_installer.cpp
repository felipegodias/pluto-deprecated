#include <pluto/asset/asset_installer.h>
#include <pluto/asset/asset_manager.h>
#include <pluto/asset/mesh.h>
#include <pluto/di/di_container.h>

namespace pluto
{
    void AssetInstaller::Install(std::string assetsDirectoryName, DiContainer& diContainer)
    {
        diContainer.AddSingleton(std::make_unique<Mesh::Factory>(diContainer));

        const AssetManager::Factory factory(diContainer);
        diContainer.AddSingleton(factory.Create(std::move(assetsDirectoryName)));
    }

    void AssetInstaller::Uninstall(DiContainer& diContainer)
    {
        diContainer.RemoveSingleton<AssetManager>();
        diContainer.RemoveSingleton<Mesh::Factory>();
    }
}
