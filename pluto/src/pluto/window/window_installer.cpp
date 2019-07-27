#include <pluto/window/window_installer.h>
#include <pluto/window/window_manager.h>
#include <pluto/service/service_collection.h>

namespace pluto
{
    void WindowInstaller::Install(ServiceCollection& diContainer)
    {
        const WindowManager::Factory factory(diContainer);
        diContainer.AddService(factory.Create());
    }

    void WindowInstaller::Uninstall(ServiceCollection& diContainer)
    {
        diContainer.RemoveService<WindowManager>();
    }
}
