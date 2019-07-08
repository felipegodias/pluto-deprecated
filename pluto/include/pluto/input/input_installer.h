#pragma once

#include "../api.h"

namespace pluto
{
    class DiContainer;

    class PLUTO_API InputInstaller
    {
    public:
        static void Install(DiContainer& diContainer);
        static void Uninstall(DiContainer& diContainer);
    };
}
