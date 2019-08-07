#include <pluto/input/input_installer.h>
#include <pluto/input/input_manager.h>
#include <pluto/service/service_collection.h>

namespace pluto
{
    void InputInstaller::Install(ServiceCollection& serviceCollection)
    {
        const InputManager::Factory factory(serviceCollection);
        serviceCollection.AddService(factory.Create());
    }

    void InputInstaller::Uninstall(ServiceCollection& serviceCollection)
    {
        serviceCollection.RemoveService<InputManager>();
    }
}
