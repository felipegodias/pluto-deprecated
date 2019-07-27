#pragma once

#include "../api.h"

namespace pluto
{
    class ServiceCollection;

    class PLUTO_API EventInstaller
    {
    public:
        static void Install(ServiceCollection& diContainer);
        static void Uninstall(ServiceCollection& diContainer);
    };
}
