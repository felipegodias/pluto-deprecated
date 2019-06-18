#pragma once

#include "../api.h"
#include <string>

namespace pluto
{
    class DiContainer;

    class PLUTO_API LogInstaller
    {
    public:
        static void Install(const std::string& logFileName, DiContainer& diContainer);
        static void Uninstall(DiContainer& diContainer);
    };
}
