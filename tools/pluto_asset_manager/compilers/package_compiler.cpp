#include "package_compiler.h"

#include <pluto/asset/package_manifest_asset.h>
#include <pluto/regex.h>

#include <pluto/file/file_manager.h>
#include <pluto/file/file_writer.h>
#include <pluto/file/path.h>

#include <vector>
#include <memory>

namespace pluto::compiler
{
    PackageCompiler::PackageCompiler(PackageManifestAsset::Factory& packageManifestAssetFactory,
                                     const std::vector<std::reference_wrapper<BaseCompiler>>& compilers)
        : packageManifestAssetFactory(&packageManifestAssetFactory)
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

        FileManager::SetRootPath(input);

        FileManager::Delete(outputDir);

        const Regex plutoFileFilter("^.*pluto$");
        std::vector<std::string> plutoFiles = FileManager::GetFiles("", plutoFileFilter,
                                                                    FileManager::SearchOptions::AllDirectories);

        FileManager::CreateDirectory(outputDir);

        auto packageManifest = packageManifestAssetFactory->Create();

        packageManifest->SetName(Path::GetFileName(input));

        for (const std::string& plutoFile : plutoFiles)
        {
            std::string filePath = Path::RemoveExtension(plutoFile);

            std::string fileExtension = Path::GetExtension(filePath);
            BaseCompiler* compiler = compilers.at(fileExtension);
            std::vector<CompiledAsset> currentAssets = compiler->Compile(filePath, outputDir);
            for (auto& it : currentAssets)
            {
                assets.push_back(it);
                packageManifest->AddAsset(Path::Normalize(it.virtualPath), it.assetId);
            }
        }

        FileWriter fileWriter = FileManager::OpenWrite(Path::Combine({
            packageManifest->GetName(), packageManifest->GetName()
        }));

        packageManifest->Dump(fileWriter);

        return assets;
    }
}
