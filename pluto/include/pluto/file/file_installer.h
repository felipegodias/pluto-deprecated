#pragma once

#include "pluto/api.h"

namespace pluto
{
    class ServiceCollection;

    class PLUTO_API FileInstaller
    {
    public:
        static void Install(ServiceCollection& serviceCollection);
        static void Uninstall(ServiceCollection& serviceCollection);
    };
}
