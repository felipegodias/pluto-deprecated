#include <pluto/input/input_installer.h>
#include <pluto/input/input_manager.h>
#include <pluto/di/di_container.h>

namespace pluto
{
    void InputInstaller::Install(DiContainer& diContainer)
    {
        const InputManager::Factory factory(diContainer);
        diContainer.AddSingleton(factory.Create());
    }

    void InputInstaller::Uninstall(DiContainer& diContainer)
    {
        diContainer.RemoveSingleton<InputManager>();
    }
}
