#pragma once

#include "../api.h"
#include <string>

namespace pluto
{
    class DiContainer;

    class PLUTO_API FileInstaller
    {
    public:
        static void Install(const std::string& dataDirectoryName, DiContainer& diContainer);
        static void Uninstall(DiContainer& diContainer);
    };
}
