#include <pluto/simulation/simulation_installer.h>
#include <pluto/simulation/simulation_manager.h>
#include <pluto/service/service_collection.h>

namespace pluto
{
    void SimulationInstaller::Install(ServiceCollection& diContainer)
    {
        const SimulationManager::Factory factory(diContainer);
        diContainer.AddSingleton(factory.Create());
    }

    void SimulationInstaller::Uninstall(ServiceCollection& diContainer)
    {
        diContainer.RemoveSingleton<SimulationManager>();
    }
}
