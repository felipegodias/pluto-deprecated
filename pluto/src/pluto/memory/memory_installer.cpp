#include "pluto/memory/memory_installer.h"
#include "pluto/memory/memory_manager.h"
#include "pluto/memory/resource_control.h"

#include "pluto/service/service_collection.h"

namespace pluto
{
    void MemoryInstaller::Install(ServiceCollection& serviceCollection)
    {
        serviceCollection.AddFactory<ResourceControl>(std::make_unique<ResourceControl::Factory>(serviceCollection));
        serviceCollection.AddService(MemoryManager::Factory(serviceCollection).Create());
    }

    void MemoryInstaller::Uninstall(ServiceCollection& serviceCollection)
    {
        serviceCollection.RemoveService<MemoryManager>();
        serviceCollection.RemoveFactory<ResourceControl>();
    }
}
