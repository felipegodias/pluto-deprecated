#pragma once

#include "pluto/api.h"

namespace pluto
{
    class ServiceCollection;

    class PLUTO_API RenderInstaller
    {
    public:
        static void Install(ServiceCollection& diContainer);
        static void Uninstall(ServiceCollection& diContainer);
    };
}
