#pragma once

#include "../api.h"

namespace pluto
{
    class ServiceCollection;

    class PLUTO_API SimulationInstaller
    {
    public:
        static void Install(ServiceCollection& serviceCollection);
        static void Uninstall(ServiceCollection& serviceCollection);
    };
}
