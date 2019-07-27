#pragma once

#include "../api.h"

namespace pluto
{
    class ServiceCollection;
    class FileReader;

    class PLUTO_API ConfigInstaller
    {
    public:
        static void Install(FileReader* configFile, ServiceCollection& diContainer);
        static void Uninstall(ServiceCollection& diContainer);
    };
}
