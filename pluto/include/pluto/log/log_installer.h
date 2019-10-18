#pragma once

#include "pluto/api.h"
#include <memory>

namespace pluto
{
    class ServiceCollection;
    class FileStreamWriter;

    class PLUTO_API LogInstaller
    {
    public:
        static void Install(std::unique_ptr<FileStreamWriter> logFile, ServiceCollection& serviceCollection);
        static void Uninstall(ServiceCollection& serviceCollection);
    };
}
