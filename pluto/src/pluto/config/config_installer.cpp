#include <pluto/config/config_installer.h>
#include <pluto/config/config_manager.h>
#include <pluto/service/service_collection.h>

namespace pluto
{
    void ConfigInstaller::Install(FileStreamReader* configFile, ServiceCollection& serviceCollection)
    {
        const ConfigManager::Factory factory(serviceCollection);
        serviceCollection.AddService(factory.Create(configFile));
    }

    void ConfigInstaller::Uninstall(ServiceCollection& serviceCollection)
    {
        serviceCollection.RemoveService<ConfigManager>();
    }
}
