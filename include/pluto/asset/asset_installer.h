#pragma once

#include "../api.h"
#include <string>

namespace pluto
{
    class DiContainer;

    class PLUTO_API AssetInstaller
    {
    public:
        static void Install(DiContainer& diContainer);
        static void Uninstall(DiContainer& diContainer);
    };
}
