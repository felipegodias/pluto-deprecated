#include <pluto/input/input_installer.h>
#include <pluto/input/input_manager.h>
#include <pluto/service/service_collection.h>

namespace pluto
{
    void InputInstaller::Install(ServiceCollection& diContainer)
    {
        const InputManager::Factory factory(diContainer);
        diContainer.AddService(factory.Create());
    }

    void InputInstaller::Uninstall(ServiceCollection& diContainer)
    {
        diContainer.RemoveService<InputManager>();
    }
}
