#pragma once

#include "../api.h"
#include <string>

namespace pluto
{
    class DiContainer;

    class PLUTO_API ConfigInstaller
    {
    public:
        static void Install(const std::string& configFileName, DiContainer& diContainer);
        static void Uninstall(DiContainer& diContainer);
    };
}
