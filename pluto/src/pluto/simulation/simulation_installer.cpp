#include <pluto/simulation/simulation_installer.h>
#include <pluto/simulation/simulation_manager.h>
#include <pluto/service/service_collection.h>

namespace pluto
{
    void SimulationInstaller::Install(ServiceCollection& serviceCollection)
    {
        const SimulationManager::Factory factory(serviceCollection);
        serviceCollection.AddService(factory.Create());
    }

    void SimulationInstaller::Uninstall(ServiceCollection& serviceCollection)
    {
        serviceCollection.RemoveService<SimulationManager>();
    }
}
