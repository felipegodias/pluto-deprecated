#include <pluto/root.h>
#include <pluto/service/service_collection.h>

#include <pluto/log/log_installer.h>
#include <pluto/config/config_installer.h>
#include <pluto/event/event_installer.h>
#include <pluto/window/window_installer.h>
#include <pluto/input/input_installer.h>
#include <pluto/simulation/simulation_installer.h>
#include <pluto/memory/memory_installer.h>
#include <pluto/asset/asset_installer.h>
#include <pluto/scene/scene_installer.h>
#include <pluto/render/render_installer.h>

#include <pluto/log/log_manager.h>
#include <pluto/event/event_manager.h>
#include <pluto/window/window_manager.h>
#include <pluto/simulation/simulation_manager.h>

#include <pluto/asset/asset_manager.h>

#include <pluto/file/file_manager.h>
#include <pluto/file/file_reader.h>
#include <pluto/file/file_writer.h>
#include <pluto/file/path.h>

#include <regex>
#include <pluto/exception.h>

namespace pluto
{
    class Root::Impl
    {
        std::unique_ptr<ServiceCollection> serviceCollection;

    public:
        Impl(const std::string& configFileName, const std::string& logFileName,
             const std::string& dataDirectoryName)
        {
            serviceCollection = std::make_unique<ServiceCollection>();

            FileManager::SetRootPath(dataDirectoryName);
            FileWriter logFile = FileManager::OpenWrite(logFileName);
            LogInstaller::Install(std::make_unique<FileWriter>(std::move(logFile)), *serviceCollection);

            if (FileManager::Exists(configFileName))
            {
                FileReader configFile = FileManager::OpenRead(configFileName);
                ConfigInstaller::Install(&configFile, *serviceCollection);
            }
            else
            {
                ConfigInstaller::Install(nullptr, *serviceCollection);
            }

            EventInstaller::Install(*serviceCollection);
            WindowInstaller::Install(*serviceCollection);
            InputInstaller::Install(*serviceCollection);
            SimulationInstaller::Install(*serviceCollection);
            MemoryInstaller::Install(*serviceCollection);
            AssetInstaller::Install(*serviceCollection);
            SceneInstaller::Install(*serviceCollection);
            RenderInstaller::Install(*serviceCollection);

            auto& logManager = serviceCollection->GetService<LogManager>();
            logManager.LogInfo("Pluto Engine Initialized!");

            auto& assetManager = serviceCollection->GetService<AssetManager>();
            assetManager.LoadPackage("built-in");
        }

        ~Impl()
        {
            RenderInstaller::Uninstall(*serviceCollection);
            SceneInstaller::Uninstall(*serviceCollection);
            AssetInstaller::Uninstall(*serviceCollection);
            MemoryInstaller::Uninstall(*serviceCollection);
            SimulationInstaller::Uninstall(*serviceCollection);
            InputInstaller::Uninstall(*serviceCollection);
            WindowInstaller::Uninstall(*serviceCollection);
            EventInstaller::Uninstall(*serviceCollection);
            ConfigInstaller::Uninstall(*serviceCollection);
            LogInstaller::Uninstall(*serviceCollection);
        }

        int Run(const std::function<void(ServiceCollection& serviceCollection)>& onInit) const
        {
            auto& windowManager = serviceCollection->GetService<WindowManager>();
            auto& simulationManager = serviceCollection->GetService<SimulationManager>();
            auto& logManager = serviceCollection->GetService<LogManager>();

            onInit(*serviceCollection);

            int i = 0;
            while (windowManager.IsOpen())
            {
                try
                {
                    simulationManager.Run();
                    ++i;
                }
                catch (const Exception& e)
                {
                    logManager.LogException(e);
                }
            }
            return 0;
        }
    };

    Root::Root(const std::string& configFileName, const std::string& logFileName,
               const std::string& dataDirectoryName)
        : impl(
            std::make_unique<Impl>(configFileName, logFileName, dataDirectoryName))
    {
    }

    Root::~Root() = default;

    int Root::Run(const std::function<void(ServiceCollection& serviceCollection)>& onInit) const
    {
        return impl->Run(onInit);
    }
}
