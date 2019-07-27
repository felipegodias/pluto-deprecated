#include <pluto/config/config_installer.h>
#include <pluto/config/config_manager.h>
#include <pluto/service/service_collection.h>

namespace pluto
{
    void ConfigInstaller::Install(FileReader* configFile, ServiceCollection& diContainer)
    {
        const ConfigManager::Factory factory(diContainer);
        diContainer.AddSingleton(factory.Create(configFile));
    }

    void ConfigInstaller::Uninstall(ServiceCollection& diContainer)
    {
        diContainer.RemoveSingleton<ConfigManager>();
    }
}
