#include <pluto/window/window_installer.h>
#include <pluto/window/window_manager.h>
#include <pluto/di/di_container.h>

namespace pluto
{
    void WindowInstaller::Install(DiContainer& diContainer)
    {
        const WindowManager::Factory factory(diContainer);
        diContainer.AddSingleton(factory.Create());
    }

    void WindowInstaller::Uninstall(DiContainer& diContainer)
    {
        diContainer.RemoveSingleton<WindowManager>();
    }
}
