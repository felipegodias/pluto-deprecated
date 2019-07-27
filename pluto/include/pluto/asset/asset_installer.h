#pragma once

#include "../api.h"
#include <string>

namespace pluto
{
    class ServiceCollection;

    class PLUTO_API AssetInstaller
    {
    public:
        static void Install(ServiceCollection& diContainer);
        static void Uninstall(ServiceCollection& diContainer);
    };
}
