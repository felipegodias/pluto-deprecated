#include "pluto/memory/memory_installer.h"
#include "pluto/memory/memory_manager.h"
#include "pluto/memory/lazy_ptr.h"

#include "pluto/service/service_collection.h"

namespace pluto
{
    void MemoryInstaller::Install(ServiceCollection& serviceCollection)
    {
        serviceCollection.AddService(std::make_unique<LazyPtr::Factory>(serviceCollection));
        serviceCollection.AddService(MemoryManager::Factory(serviceCollection).Create());
    }

    void MemoryInstaller::Uninstall(ServiceCollection& serviceCollection)
    {
        serviceCollection.RemoveService<MemoryManager>();
        serviceCollection.RemoveService<LazyPtr::Factory>();
    }
}
