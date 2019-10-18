#pragma once

#include "../api.h"

namespace pluto
{
    class ServiceCollection;
    class FileStreamReader;

    class PLUTO_API ConfigInstaller
    {
    public:
        static void Install(FileStreamReader* configFile, ServiceCollection& serviceCollection);
        static void Uninstall(ServiceCollection& serviceCollection);
    };
}
