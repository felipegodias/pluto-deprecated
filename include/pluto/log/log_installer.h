#pragma once

#include "../api.h"
#include <memory>

namespace pluto
{
    class DiContainer;
    class FileWriter;

    class PLUTO_API LogInstaller
    {
    public:
        static void Install(std::unique_ptr<FileWriter> logFile, DiContainer& diContainer);
        static void Uninstall(DiContainer& diContainer);
    };
}
