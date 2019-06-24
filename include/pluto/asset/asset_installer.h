#pragma once

#include "../api.h"
#include <string>

namespace pluto
{
    class DiContainer;

    class PLUTO_API AssetInstaller
    {
    public:
        static void Install(std::string assetsDirectoryName, DiContainer& diContainer);
        static void Uninstall(DiContainer& diContainer);
    };
}
