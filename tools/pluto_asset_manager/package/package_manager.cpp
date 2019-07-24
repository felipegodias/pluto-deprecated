#include "package_manager.h"
#include "../mesh/mesh_asset_manager.h"
#include "../shader/shader_asset_manager.h"
#include "../text/text_asset_manager.h"

#include <pluto/asset/asset.h>
#include <pluto/asset/package_manifest_asset.h>
#include <pluto/regex.h>

#include <pluto/file/file_manager.h>
#include <pluto/file/file_writer.h>
#include <pluto/file/path.h>
#include <pluto/guid.h>

#include <iostream>
#include <vector>
#include <memory>

namespace pluto
{
    PackageManager::PackageManager(FileManager& fileManager, PackageManifestAsset::Factory& packageManifestAssetFactory,
                                   TextAssetManager& textAssetManager, MeshAssetManager& meshAssetManager,
                                   ShaderAssetManager& shaderAssetManager) : fileManager(&fileManager),
                                                                             packageManifestAssetFactory(
                                                                                 &packageManifestAssetFactory),
                                                                             textAssetManager(&textAssetManager),
                                                                             meshAssetManager(&meshAssetManager),
                                                                             shaderAssetManager(&shaderAssetManager)
    {
    }

    void PackageManager::Create(const Path& path)
    {
        fileManager->SetRootPath(path);

        fileManager->Delete(Path(path.GetName()));

        const Regex plutoFileFilter("^.*pluto$");
        std::vector<Path> plutoFiles = fileManager->GetFiles(path, plutoFileFilter, FileManager::AllDirectories);

        fileManager->CreateDirectory(Path(path.GetName()));

        auto packageManifest = packageManifestAssetFactory->Create();

        packageManifest->SetName(path.GetName());

        for (const Path& plutoFile : plutoFiles)
        {
            Path file = plutoFile;

            // Removes the .pluto extension.
            file.RemoveExtension();

            std::string fileExtension = file.GetExtension();
            std::unique_ptr<Asset> asset = nullptr;
            if (fileExtension == ".glsl")
            {
                asset = shaderAssetManager->Create(file);
            }
            else if (fileExtension == ".obj")
            {
                asset = meshAssetManager->Create(file);
            }
            else if (fileExtension == ".txt")
            {
                asset = textAssetManager->Create(file);
            }

            if (asset != nullptr)
            {
                packageManifest->AddAsset(file.GetRelativePath(path).Str(), asset->GetId());
                const auto fileWriter = fileManager->OpenWrite(Path(path.GetName() + "/" + asset->GetId().Str()));
                asset->Dump(*fileWriter);
                std::cout << "Asset \"" << asset->GetName() << "\" saved with id " << asset->GetId() << "." << std::
                    endl;
            }
        }

        const auto fileWriter = fileManager->OpenWrite(Path(path.GetName() + "/" + packageManifest->GetName()));
        packageManifest->Dump(*fileWriter);
        std::cout << "Asset \"" << packageManifest->GetName() << "\" saved with id " << packageManifest->GetId() << "."
            << std::endl;
    }
}
