#include <pluto/root.h>
#include <pluto/log/log_manager.h>
#include <iostream>

namespace pluto
{
    class Root::Impl
    {
    private:
        std::unique_ptr<LogManager> logManager;

    public:
        Impl(const std::string& configFileName, const std::string& logFileName,
             const std::string& assetsDirectoryName)
        {
            logManager = LogManager::Factory::Create(logFileName);
            logManager->LogInfo("Pluto Engine Initialized!");
        }

        ~Impl()
        {
            logManager.reset();
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
