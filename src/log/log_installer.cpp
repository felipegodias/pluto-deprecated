#include <pluto/log/log_installer.h>
#include <pluto/log/log_manager.h>
#include <pluto/di/di_container.h>

namespace pluto
{
    void LogInstaller::Install(const std::string& logFileName, DiContainer& diContainer)
    {
        const LogManager::Factory factory(diContainer);
        diContainer.AddSingleton<LogManager>(factory.Create(logFileName));
    }

    void LogInstaller::Uninstall(DiContainer& diContainer)
    {
        diContainer.RemoveSingleton<LogManager>();
    }
}
