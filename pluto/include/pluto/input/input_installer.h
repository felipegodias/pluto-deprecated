#pragma once

#include "../api.h"

namespace pluto
{
    class ServiceCollection;

    class PLUTO_API InputInstaller
    {
    public:
        static void Install(ServiceCollection& serviceCollection);
        static void Uninstall(ServiceCollection& serviceCollection);
    };
}
