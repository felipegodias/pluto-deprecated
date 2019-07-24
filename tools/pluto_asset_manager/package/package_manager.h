#pragma once

#include <memory>

namespace pluto
{
    class DiContainer;
    class PackageManifestAsset;
    class Path;

    class PackageManager
    {
        DiContainer* diContainer;

    public:
        explicit PackageManager(DiContainer& diContainer);

        std::unique_ptr<PackageManifestAsset> Create(const Path& path);
    };
}
