#pragma once

#include "../api.h"

namespace pluto
{
    class DiContainer;

    class PLUTO_API SimulationInstaller
    {
    public:
        static void Install(DiContainer& diContainer);
        static void Uninstall(DiContainer& diContainer);
    };
}
