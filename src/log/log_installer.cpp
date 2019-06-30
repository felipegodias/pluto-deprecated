#include <pluto/log/log_installer.h>
#include <pluto/log/log_manager.h>
#include <pluto/file/file_writer.h>
#include <pluto/di/di_container.h>

namespace pluto
{
    void LogInstaller::Install(std::unique_ptr<FileWriter> logFile, DiContainer& diContainer)
    {
        const LogManager::Factory factory(diContainer);
        diContainer.AddSingleton<LogManager>(factory.Create(std::move(logFile)));
    }

    void LogInstaller::Uninstall(DiContainer& diContainer)
    {
        diContainer.RemoveSingleton<LogManager>();
    }
}
