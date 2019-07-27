#pragma once

#include "../api.h"
#include <memory>

namespace pluto
{
    class ServiceCollection;
    class FileWriter;

    class PLUTO_API LogInstaller
    {
    public:
        static void Install(std::unique_ptr<FileWriter> logFile, ServiceCollection& diContainer);
        static void Uninstall(ServiceCollection& diContainer);
    };
}
