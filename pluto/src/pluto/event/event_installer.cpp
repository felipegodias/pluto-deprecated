#include <pluto/event/event_installer.h>
#include <pluto/event/event_manager.h>
#include <pluto/di/di_container.h>

namespace pluto
{
    void EventInstaller::Install(DiContainer& diContainer)
    {
        const EventManager::Factory factory(diContainer);
        diContainer.AddSingleton(factory.Create());
    }

    void EventInstaller::Uninstall(DiContainer& diContainer)
    {
        diContainer.RemoveSingleton<EventManager>();
    }
}
