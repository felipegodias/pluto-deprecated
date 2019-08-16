#include "package_compiler.h"

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

namespace pluto::compiler
{
    PackageCompiler::PackageCompiler(FileManager& fileManager,
                                     PackageManifestAsset::Factory& packageManifestAssetFactory,
                                     const std::vector<std::reference_wrapper<BaseCompiler>>& compilers)
        : fileManager(&fileManager),
          packageManifestAssetFactory(&packageManifestAssetFactory)
    {
        for (auto& it : compilers)
        {
            BaseCompiler& compiler = it;
            for (auto& extension : compiler.GetExtensions())
            {
                this->compilers.emplace(extension, &compiler);
            }
        }
    }

    std::vector<std::string> PackageCompiler::GetExtensions() const
    {
        return {};
    }

    std::vector<BaseCompiler::CompiledAsset> PackageCompiler::Compile(const std::string& input,
                                                                      const std::string& outputDir) const
    {
        std::vector<CompiledAsset> assets;

        const Path path(input);
        fileManager->SetRootPath(path);

        const Path outputDirPath = Path(outputDir);
        fileManager->Delete(outputDirPath);

        const Regex plutoFileFilter("^.*pluto$");
        std::vector<Path> plutoFiles = fileManager->GetFiles(path, plutoFileFilter,
                                                             FileManager::SearchOptions::AllDirectories);

        fileManager->CreateDirectory(outputDirPath);

        auto packageManifest = packageManifestAssetFactory->Create();

        packageManifest->SetName(path.GetName());

        for (const Path& plutoFile : plutoFiles)
        {
            Path file = plutoFile;

            // Removes the .pluto extension.
            file.RemoveExtension();

            std::string fileExtension = file.GetExtension();
            BaseCompiler* compiler = compilers.at(fileExtension);
            std::vector<CompiledAsset> currentAssets = compiler->Compile(file.Str(), outputDir);
            for (auto& it : currentAssets)
            {
                assets.push_back(it);
                packageManifest->AddAsset(it.virtualPath, it.assetId);
            }
        }

        const auto fileWriter = fileManager->OpenWrite(Path(path.GetName() + "/" + packageManifest->GetName()));
        packageManifest->Dump(*fileWriter);
        std::cout << "Asset \"" << packageManifest->GetName() << "\" saved with id " << packageManifest->GetId() << "."
            << std::endl;

        return assets;
    }
}
