#include <pluto/config/config_installer.h>
#include <pluto/config/config_manager.h>
#include <pluto/di/di_container.h>

namespace pluto
{
    void ConfigInstaller::Install(FileReader* configFile, DiContainer& diContainer)
    {
        const ConfigManager::Factory factory(diContainer);
        diContainer.AddSingleton(factory.Create(configFile));
    }

    void ConfigInstaller::Uninstall(DiContainer& diContainer)
    {
        diContainer.RemoveSingleton<ConfigManager>();
    }
}
