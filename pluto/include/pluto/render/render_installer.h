#pragma once

#include "pluto/api.h"

namespace pluto
{
    class DiContainer;

    class PLUTO_API RenderInstaller
    {
    public:
        static void Install(DiContainer& diContainer);
        static void Uninstall(DiContainer& diContainer);
    };
}
