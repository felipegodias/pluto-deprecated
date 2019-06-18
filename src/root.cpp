#include <pluto/root.h>
#include <pluto/di/di_container.h>
#include <pluto/log/log_manager.h>
#include <pluto/log/log_installer.h>
#include <pluto/config/config_installer.h>

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

            LogInstaller::Install(logFileName, *diContainer);
            ConfigInstaller::Install(configFileName, *diContainer);

            logManager = &diContainer->Resolve<LogManager>();
            logManager->LogInfo("Pluto Engine Initialized!");
        }

        ~Impl()
        {
            ConfigInstaller::Uninstall(*diContainer);
            LogInstaller::Uninstall(*diContainer);
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
