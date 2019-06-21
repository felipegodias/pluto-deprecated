#include <pluto/root.h>
#include <pluto/di/di_container.h>

#include <pluto/log/log_installer.h>
#include <pluto/config/config_installer.h>
#include <pluto/file/file_installer.h>
#include <pluto/event/event_installer.h>
#include <pluto/window/window_installer.h>
#include <pluto/input/input_installer.h>
#include <pluto/simulation/simulation_installer.h>

#include <pluto/log/log_manager.h>
#include <pluto/event/event_manager.h>
#include <pluto/event/on_startup_event.h>
#include <pluto/window/window_manager.h>
#include <pluto/simulation/simulation_manager.h>

#include <pluto/math/vector2.h>

namespace pluto
{
    class Root::Impl
    {
    private:
        std::unique_ptr<DiContainer> diContainer;

    public:
        Impl(const std::string& configFileName, const std::string& logFileName,
             const std::string& assetsDirectoryName)
        {
            diContainer = std::make_unique<DiContainer>();

            LogInstaller::Install(logFileName, *diContainer);
            ConfigInstaller::Install(configFileName, *diContainer);
            FileInstaller::Install(*diContainer);
            EventInstaller::Install(*diContainer);
            WindowInstaller::Install(*diContainer);
            InputInstaller::Install(*diContainer);
            SimulationInstaller::Install(*diContainer);

            auto& logManager = diContainer->GetSingleton<LogManager>();
            logManager.LogInfo("Pluto Engine Initialized!");

            Vector2 b = Vector2::RIGHT * 2;
            Vector2 c = Vector2::ClampMagnitude(b, 1, 1.5);

            auto& eventManager = diContainer->GetSingleton<EventManager>();
            eventManager.Dispatch(OnStartupEvent());
        }

        ~Impl()
        {
            SimulationInstaller::Uninstall(*diContainer);
            InputInstaller::Uninstall(*diContainer);
            WindowInstaller::Uninstall(*diContainer);
            EventInstaller::Uninstall(*diContainer);
            FileInstaller::Uninstall(*diContainer);
            ConfigInstaller::Uninstall(*diContainer);
            LogInstaller::Uninstall(*diContainer);
        }

        int Run() const
        {
            auto& windowManager = diContainer->GetSingleton<WindowManager>();
            auto& simulationManager = diContainer->GetSingleton<SimulationManager>();
            while (windowManager.IsOpen())
            {
                simulationManager.Run();
            }
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
