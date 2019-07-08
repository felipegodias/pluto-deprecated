#pragma once

#include "../api.h"

namespace pluto
{
    class DiContainer;
    class FileReader;

    class PLUTO_API ConfigInstaller
    {
    public:
        static void Install(FileReader* configFile, DiContainer& diContainer);
        static void Uninstall(DiContainer& diContainer);
    };
}
