#include <pluto/root.h>
#include <pluto/service/service_collection.h>

#include <pluto/file/file_installer.h>
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
#include <pluto/physics_2d/physics_2d_installer.h>

#include <pluto/log/log_manager.h>
#include <pluto/event/event_manager.h>
#include <pluto/window/window_manager.h>
#include <pluto/simulation/simulation_manager.h>

#include <pluto/asset/asset_manager.h>

#include <pluto/file/file_manager.h>
#include <pluto/file/file_stream_reader.h>
#include <pluto/file/file_stream_writer.h>

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

            FileInstaller::Install(*serviceCollection);
            auto& fileManager = serviceCollection->GetService<FileManager>();

            fileManager.SetRootPath(dataDirectoryName);
            std::unique_ptr<FileStreamWriter> logFile = fileManager.OpenWrite(logFileName);
            LogInstaller::Install(std::move(logFile), *serviceCollection);

            if (fileManager.IsFile(configFileName))
            {
                const std::unique_ptr<FileStreamReader> configFile = fileManager.OpenRead(configFileName);
                ConfigInstaller::Install(configFile.get(), *serviceCollection);
            }
            else
            {
                ConfigInstaller::Install(nullptr, *serviceCollection);
            }

            EventInstaller::Install(*serviceCollection);
            WindowInstaller::Install(*serviceCollection);
            InputInstaller::Install(*serviceCollection);
            MemoryInstaller::Install(*serviceCollection);
            AssetInstaller::Install(*serviceCollection);
            SceneInstaller::Install(*serviceCollection);
            RenderInstaller::Install(*serviceCollection);
            Physics2DInstaller::Install(*serviceCollection);
            SimulationInstaller::Install(*serviceCollection);

            auto& logManager = serviceCollection->GetService<LogManager>();
            logManager.LogInfo("Pluto Engine Initialized!");

            auto& assetManager = serviceCollection->GetService<AssetManager>();
            assetManager.LoadPackage("built-in");
        }

        ~Impl()
        {
            SimulationInstaller::Uninstall(*serviceCollection);
            SceneInstaller::Uninstall(*serviceCollection);
            Physics2DInstaller::Uninstall(*serviceCollection);
            RenderInstaller::Uninstall(*serviceCollection);
            AssetInstaller::Uninstall(*serviceCollection);
            MemoryInstaller::Uninstall(*serviceCollection);
            InputInstaller::Uninstall(*serviceCollection);
            WindowInstaller::Uninstall(*serviceCollection);
            EventInstaller::Uninstall(*serviceCollection);
            ConfigInstaller::Uninstall(*serviceCollection);
            LogInstaller::Uninstall(*serviceCollection);
        }

        int Run(const std::function<void(ServiceCollection& serviceCollection)>& onSetup,
                const std::function<void(ServiceCollection& serviceCollection)>& onTeardown) const
        {
            auto& windowManager = serviceCollection->GetService<WindowManager>();
            auto& simulationManager = serviceCollection->GetService<SimulationManager>();
            auto& logManager = serviceCollection->GetService<LogManager>();

            int exitCode = EXIT_SUCCESS;
            try
            {
                onSetup(*serviceCollection);
                while (windowManager.IsOpen())
                {
                    simulationManager.MainLoop();
                }
                onTeardown(*serviceCollection);
            }
            catch (const Exception& e)
            {
                logManager.LogException(e);
                exitCode = EXIT_FAILURE;
            }
            return exitCode;
        }
    };

    Root::Root(const std::string& configFileName, const std::string& logFileName,
               const std::string& dataDirectoryName)
        : impl(
            std::make_unique<Impl>(configFileName, logFileName, dataDirectoryName))
    {
    }

    Root::~Root() = default;

    int Root::Run(const std::function<void(ServiceCollection& serviceCollection)>& onSetup,
                  const std::function<void(ServiceCollection& serviceCollection)>& onTeardown) const
    {
        return impl->Run(onSetup, onTeardown);
    }
}
