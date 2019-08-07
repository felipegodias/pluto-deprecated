#pragma once

#include "../api.h"
#include <string>

namespace pluto
{
    class ServiceCollection;

    class PLUTO_API AssetInstaller
    {
    public:
        static void Install(ServiceCollection& serviceCollection);
        static void Uninstall(ServiceCollection& serviceCollection);
    };
}
