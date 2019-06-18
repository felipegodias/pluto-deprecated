#pragma once

#include "../api.h"

namespace pluto
{
    class DiContainer;

    class PLUTO_API EventInstaller
    {
    public:
        static void Install(DiContainer& diContainer);
        static void Uninstall(DiContainer& diContainer);
    };
}
