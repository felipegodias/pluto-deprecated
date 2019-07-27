#include <pluto/event/event_installer.h>
#include <pluto/event/event_manager.h>
#include <pluto/service/service_collection.h>

namespace pluto
{
    void EventInstaller::Install(ServiceCollection& diContainer)
    {
        const EventManager::Factory factory(diContainer);
        diContainer.AddSingleton(factory.Create());
    }

    void EventInstaller::Uninstall(ServiceCollection& diContainer)
    {
        diContainer.RemoveSingleton<EventManager>();
    }
}
