#include <pluto/event/event_installer.h>
#include <pluto/event/event_manager.h>
#include <pluto/service/service_collection.h>

namespace pluto
{
    void EventInstaller::Install(ServiceCollection& serviceCollection)
    {
        const EventManager::Factory factory(serviceCollection);
        serviceCollection.AddService(factory.Create());
    }

    void EventInstaller::Uninstall(ServiceCollection& serviceCollection)
    {
        serviceCollection.RemoveService<EventManager>();
    }
}
