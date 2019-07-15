#pragma once

#include <string>
#include <memory>

namespace pluto
{
    class PackageManifestAsset;
    class Guid;

    void CreatePackage(const std::string& path);
    std::unique_ptr<PackageManifestAsset> LoadPackageManifest(const Guid& guid);
}
