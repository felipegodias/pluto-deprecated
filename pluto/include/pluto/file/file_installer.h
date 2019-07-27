#pragma once

#include "../api.h"
#include <string>

namespace pluto
{
    class ServiceCollection;

    class PLUTO_API FileInstaller
    {
    public:
        static void Install(const std::string& dataDirectoryName, ServiceCollection& diContainer);
        static void Uninstall(ServiceCollection& diContainer);
    };
}
