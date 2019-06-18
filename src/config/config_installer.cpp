#include <pluto/config/config_installer.h>
#include <pluto/config/config_manager.h>
#include <pluto/di/di_container.h>

namespace pluto
{
    void ConfigInstaller::Install(const std::string& configFileName, DiContainer& diContainer)
    {
        ConfigManager::Factory factory(diContainer);
        diContainer.AddSingleton(factory.Create(configFileName));
    }

    void ConfigInstaller::Uninstall(DiContainer& diContainer)
    {
        diContainer.RemoveSingleton<ConfigManager>();
    }
}
