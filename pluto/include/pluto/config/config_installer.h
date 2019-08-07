#pragma once

#include "../api.h"

namespace pluto
{
    class ServiceCollection;
    class FileReader;

    class PLUTO_API ConfigInstaller
    {
    public:
        static void Install(FileReader* configFile, ServiceCollection& serviceCollection);
        static void Uninstall(ServiceCollection& serviceCollection);
    };
}
