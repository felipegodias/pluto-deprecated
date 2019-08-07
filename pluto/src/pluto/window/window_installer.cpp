#include <pluto/window/window_installer.h>
#include <pluto/window/window_manager.h>
#include <pluto/service/service_collection.h>

namespace pluto
{
    void WindowInstaller::Install(ServiceCollection& serviceCollection)
    {
        const WindowManager::Factory factory(serviceCollection);
        serviceCollection.AddService(factory.Create());
    }

    void WindowInstaller::Uninstall(ServiceCollection& serviceCollection)
    {
        serviceCollection.RemoveService<WindowManager>();
    }
}
