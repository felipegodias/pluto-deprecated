#include "package_manager.h"
#include "../mesh/mesh_asset_manager.h"
#include "../shader/shader_asset_manager.h"

#include <pluto/asset/asset.h>
#include <pluto/asset/package_manifest_asset.h>
#include <pluto/regex.h>
#include <pluto/di/di_container.h>

#include <pluto/file/file_manager.h>
#include <pluto/file/file_reader.h>
#include <pluto/file/path.h>

#include <fmt/ostream.h>
#include <vector>
#include <memory>

namespace pluto
{
    PackageManager::PackageManager(DiContainer& diContainer) : diContainer(&diContainer)
    {
    }

    std::unique_ptr<PackageManifestAsset> PackageManager::Create(const Path& path)
    {
        auto& fileManager = diContainer->GetSingleton<FileManager>();
        fileManager.SetRootPath(path);

        //fileManager.Delete(Path(packagePath.GetName()));

        const Regex plutoFileFilter("^.*pluto$");
        std::vector<Path> plutoFiles = fileManager.GetFiles(path, plutoFileFilter, FileManager::AllDirectories);

        auto packageManifest = diContainer->GetSingleton<PackageManifestAsset::Factory>().Create();

        packageManifest->SetName(path.GetName());

        fileManager.CreateDirectory(Path(path.GetName()));

        for (const Path& plutoFile : plutoFiles)
        {
            /*
            Path file = plutoFile;

            // Removes the .pluto extension.
            file.RemoveExtension();

            std::string fileExtension = file.GetExtension();
            std::unique_ptr<Asset> asset;
            if (fileExtension == ".glsl")
            {
                //asset = CreateShaderAsset(file.Str());
            }
            else if (fileExtension == ".obj")
            {
                //asset = CreateMeshAsset(file.Str());
            }

            packageManifest->AddAsset(file.Str(), asset->GetId());
            const auto fileWriter = fileManager->OpenWrite(
                Path(fmt::format("{0}/{1}", packagePath.GetName(), asset->GetId())));
            asset->Dump(*fileWriter);
            */
        }

        //const auto fileWriter = fileManager->OpenWrite(
        //    Path(fmt::format("{0}/{0}", packagePath.GetName())));
        //packageManifest->Dump(*fileWriter);

        return packageManifest;
    }
}
