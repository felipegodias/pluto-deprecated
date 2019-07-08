#include <pluto/simulation/simulation_installer.h>
#include <pluto/simulation/simulation_manager.h>
#include <pluto/di/di_container.h>

namespace pluto
{
    void SimulationInstaller::Install(DiContainer& diContainer)
    {
        const SimulationManager::Factory factory(diContainer);
        diContainer.AddSingleton(factory.Create());
    }

    void SimulationInstaller::Uninstall(DiContainer& diContainer)
    {
        diContainer.RemoveSingleton<SimulationManager>();
    }
}
