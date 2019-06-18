#include <pluto/root.h>
#include <pluto/di_container.h>
#include <pluto/log/log_manager.h>
#include <pluto/config/config_installer.h>
#include <iostream>

namespace pluto
{
    class Root::Impl
    {
    private:
        std::unique_ptr<DiContainer> diContainer;
        LogManager* logManager;

    public:
        Impl(const std::string& configFileName, const std::string& logFileName,
             const std::string& assetsDirectoryName)
        {
            diContainer = std::make_unique<DiContainer>();
            logManager = &diContainer->AddSingleton<LogManager>(LogManager::Factory::Create(logFileName));

            ConfigInstaller::Install(configFileName, *diContainer);

            logManager->LogInfo("Pluto Engine Initialized!");
        }

        ~Impl()
        {
            ConfigInstaller::Uninstall(*diContainer);
            diContainer->RemoveSingleton<LogManager>();
        }

        int Run() const
        {
            logManager->LogInfo("Hello Pluto!");
            return 0;
        }
    };

    Root::Root(const std::string& configFileName, const std::string& logFileName,
               const std::string& assetsDirectoryName) : impl(
        std::make_unique<Impl>(configFileName, logFileName, assetsDirectoryName))
    {
    }

    Root::~Root() = default;

    int Root::Run() const
    {
        return impl->Run();
    }
}
